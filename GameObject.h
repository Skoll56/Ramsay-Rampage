#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

class GameObject
{
protected:
	float m_x;
	float m_y;	
	SDL_Texture *m_tex;
	SDL_Rect m_srcRect, m_dstRect;
	float m_angle; // I don't think I ever used this, but, handy to have for if I do want to add rotation to things.

public:
	static SDL_Texture *makeTexture(const char* _directory, SDL_Renderer *_renderer); 
	GameObject(const char *_directory, SDL_Renderer *_renderer);
	GameObject() {};
	~GameObject() {};
	float getX();
	SDL_Rect* getRect() { return &m_dstRect; }
	float getY();
	void setY(float _y);
	void setX(float _x);
	void render(SDL_Renderer *_renderer);
	float getAngle() { return m_angle; }
	void setAngle(float _angle) { m_angle = _angle; }
};

struct Goal : public GameObject
{
	Goal(const char *_directory, SDL_Renderer *_renderer, int _xpos, int _ypos);
	Goal() {};
	~Goal() {};
	int m_farX, m_farY;
	int m_centreX, m_centreY;
	void update(int _permX, int _permY, int _winX, int _winY);
};


class Player: public GameObject 
{

public:
	Player(const char *_directory, SDL_Renderer *_renderer, int _winX, int _winY);
	~Player() {};
};




class Background : public GameObject
{

public:
	Background(const char *_directory, SDL_Renderer *_renderer, int _xPos, int _yPos, int _height, int _length);
	Background(const char *_directory, SDL_Renderer *_renderer, int _angle, int _length);
	Background() {};
	~Background() {};
	void update(int _x, int _y);
	void updateAndRender(int _xPermOffset, int _yPermOffset, SDL_Renderer *_renderer, int _winX, int _winY);
};

struct Text : public GameObject
{
	Text(SDL_Renderer *_renderer, int _x, int _y, int _xSize, int _ySize, int _fontSize, SDL_Color _colour, std::string _message);
	~Text() {};
	void update(std::string *_newMessage, SDL_Renderer *_renderer);
	TTF_Font *m_font;
	SDL_Color m_colour;
	void renderText(SDL_Renderer *_renderer);

	const char *m_message;

};


class Enemy : public GameObject
{
private:
	
	float m_speed = 0.0f;
	bool m_win;
	float m_centreX;
	float m_centreY;
	bool m_dead;

public:
	Enemy(const char *_directory, SDL_Renderer *_renderer, int _xpos, int _ypos);
	~Enemy();
	float getCentreX() { return m_centreX; }
	float getCentreY() { return m_centreY; }
	void setSpeed(float _speed) { m_speed = _speed; }
	float getSpeed() { return m_speed; }
	float getDead() { return m_dead; }
	void setDead(bool _dead) { m_dead = _dead; }
	void setWin(bool _win) { m_win = _win; }
	bool getWin() { return m_win; }
	void update(Enemy *_enemy, Goal *_goal, int _xOffset, int _yOffset);
};


class Splat : public GameObject
{
public:
	Splat(SDL_Rect *_dstRect, SDL_Renderer *_renderer);
	~Splat() {};

	void update(int _xOffset, int _yOffset);
};
#endif