#include "GameObject.h"
#include <SDL_ttf.h>
#include <iostream>
#include <string>

GameObject::GameObject(const char *_directory, SDL_Renderer *_renderer)
{
	m_tex = makeTexture(_directory, _renderer);
}

Player::Player(const char *_directory, SDL_Renderer *_renderer, int _winX, int _winY)
{
	m_tex = makeTexture(_directory, _renderer);
	m_srcRect.h = 1000;
	m_srcRect.w = 1000;
	m_srcRect.x = 0;
	m_srcRect.y = 0;

	m_dstRect.h = 50;
	m_dstRect.w = 50;
	
	m_dstRect.x = _winX/2 - m_dstRect.w/2; 
	m_dstRect.y = _winY/2 - m_dstRect.h/2;
}

Enemy::Enemy(const char *_directory, SDL_Renderer *_renderer, int _xpos, int _ypos)
{
	m_tex = makeTexture(_directory, _renderer);
	m_x = _xpos;
	m_y = _ypos;
	
	m_speed = ((rand() % 2)+2);
	m_srcRect.h = 250;
	m_srcRect.w = 250;
	m_dstRect.h = 50;
	m_dstRect.w = 50;
}

Enemy::~Enemy()
{

}

Goal::Goal(const char * _directory, SDL_Renderer * _renderer, int _xpos, int _ypos)
{
	m_tex = makeTexture(_directory, _renderer);
	

	m_srcRect.h = 500;
	m_srcRect.w = 500;
	m_srcRect.x = 0;
	m_srcRect.y = 0;
	m_angle = 0;

	m_dstRect.h = 150;
	m_dstRect.w = 150;
	m_dstRect.x = _xpos - m_dstRect.w / 2;
	m_dstRect.y = _ypos - m_dstRect.h / 2;
	
}
void Goal::update(int _permX, int _permY, int _winXHalf, int _winYHalf)
{
	m_dstRect.x = _winXHalf - _permX - (m_dstRect.w / 2);
	m_dstRect.y = _winYHalf - _permY - (m_dstRect.h / 2);
	m_centreX = m_dstRect.x + (m_dstRect.w / 2);
	m_centreY = m_dstRect.y + (m_dstRect.h / 2);
	m_y = m_dstRect.y; 
	m_x = m_dstRect.x; 
	m_farX = m_dstRect.x + m_dstRect.w;
	m_farY = m_dstRect.y + m_dstRect.h;
}

void GameObject::render(SDL_Renderer *_renderer)
{
	SDL_RenderCopyEx(_renderer, m_tex, &m_srcRect, &m_dstRect, m_angle, NULL, SDL_FLIP_NONE);
}
float GameObject::getX() { return m_x; }
float GameObject::getY() { return m_y; }
void GameObject::setY(float _y) { m_y = _y; }
void GameObject::setX(float _x) {m_x = _x; }

SDL_Texture* GameObject::makeTexture(const char *_directory, SDL_Renderer *_renderer) // Takes an address and the renderer, returns the texture.
{
	SDL_Surface *temp = SDL_LoadBMP(_directory);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(_renderer, temp);
	SDL_FreeSurface(temp);
	return tex;
	//This function has been adapted from the Youtube Channel "Let's Make Games"'s Tutorial on SDL.
}

Background::Background(const char *_directory, SDL_Renderer *_renderer, int _xPos, int _yPos, int _height, int _length)
{
	m_tex = makeTexture(_directory, _renderer);
	m_srcRect.h = 1000;
	m_srcRect.w = 1000;
	m_srcRect.x = 0;
	m_srcRect.y = 0;

	m_angle = 0;
	m_dstRect.h = _height;
	m_dstRect.w = _length;
	m_dstRect.x = _xPos;
	m_dstRect.y = _yPos;
}

Background::Background(const char * _directory, SDL_Renderer * _renderer, int _angle, int _length)
{
	m_tex = makeTexture(_directory, _renderer);
	m_srcRect.h = 8000;
	m_srcRect.w = 10;
	m_srcRect.x = 0;
	m_srcRect.y = 0;

	if (_directory == "border.bmp")
	{
		m_dstRect.h = _length;
		m_dstRect.w = 10;
	}
	else
	{
		m_dstRect.w = _length;
		m_dstRect.h = 10;
	}

	m_angle = _angle;
	m_dstRect.x = 0;
	m_dstRect.y = 0;
}

void Background::update(int _x, int _y)
{
	m_dstRect.x = _x;
	m_dstRect.y = _y;

} 

void Background::updateAndRender(int _xPermOffset, int _yPermOffset, SDL_Renderer * _renderer, int _winX, int _winY)
{
	for (int i = (_winX - _winX * 1.5); i <= (_winX * 2 - 500); i += 500)
	{
		for (int ii = (_winY - _winY * 1.5); ii <= (_winY*1.5); ii += 500)
		{
			m_dstRect.x = i - _xPermOffset;
			m_dstRect.y = ii - _yPermOffset;
			render(_renderer);
		} // To save memory, the same Background is rendered in several places rather than making unique objects. The background is actually several small tiled images.
	} // Multiple images has been used to allow seamless scrolling and a tiled background effect
}

void Enemy::update(Enemy *_enemy, Goal *_goal, int _xOffset, int _yOffset)
{
	
	m_dstRect.x = _enemy->getX() -_xOffset;
	m_dstRect.y = _enemy->getY() -_yOffset;
	m_centreX = m_dstRect.x + m_dstRect.w / 2;
	m_centreY = m_dstRect.y + m_dstRect.h / 2;
	float xDistance = (_goal->m_centreX - m_dstRect.w/2) - m_dstRect.x;
	float yDistance = (_goal->m_centreY - m_dstRect.h/2) - m_dstRect.y;
	float distance = sqrt((xDistance * xDistance) + (yDistance * yDistance));
	xDistance /= distance;
	yDistance /= distance;	
	_enemy->setX(m_dstRect.x + xDistance * m_speed );
	_enemy->setY(m_dstRect.y + yDistance * m_speed );

	if (m_centreX >= _goal->getX() && m_centreX <= _goal->m_farX)
	{
		if (m_centreY >= _goal->getY() && m_centreY <= _goal->m_farY) // If colliding with the Goal
		{
			m_win = true;
			m_dead = true;
		}
	}
}


Text::Text(SDL_Renderer *_renderer, int _x, int _y, int _xSize, int _ySize, int _fontSize, SDL_Color _colour, std::string _message)
{
	m_message = _message.c_str();
	m_dstRect.h = _ySize;
	m_dstRect.w = _xSize;
	m_angle = 0;
	m_dstRect.x = _x;
	m_dstRect.y = _y;
	m_font = TTF_OpenFont("arial.ttf", _fontSize); // I did not create this font. I take no credit for it.
	m_colour = _colour;
	SDL_Surface *temp = TTF_RenderText_Solid(m_font, m_message, m_colour);
	m_tex = SDL_CreateTextureFromSurface(_renderer, temp);
	SDL_FreeSurface(temp);
}

void Text::update(std::string *_newMessage, SDL_Renderer *_renderer)
{
	m_message = _newMessage->c_str();
	SDL_Surface *temp = TTF_RenderText_Solid(m_font, m_message, m_colour);
	m_tex = SDL_CreateTextureFromSurface(_renderer, temp); // Text unfortunately needs a new texture every time it updates, so the number changes. Hence why updating is only called when there is a change to be had, not every frame.
	SDL_FreeSurface(temp);
}

void Text::renderText(SDL_Renderer * _renderer) // Due to the SrcRect being NULL for text, it needs its own Render function.
{
	SDL_RenderCopy(_renderer, m_tex, NULL, &m_dstRect);
}

Splat::Splat(SDL_Rect *_dstRect, SDL_Renderer *_renderer)
{
	//m_tex = makeTexture("splat.bmp", _renderer);
	SDL_Surface *temp = SDL_LoadBMP("splat.bmp");
	uint32_t tColour = SDL_MapRGB(temp->format, 255, 255, 255);
	SDL_SetColorKey(temp, SDL_TRUE, tColour);
	m_tex = SDL_CreateTextureFromSurface(_renderer, temp);
	SDL_FreeSurface(temp);
	m_angle = 0;
	m_srcRect.x = 0;
	m_srcRect.y = 0;
	m_srcRect.w = 500;
	m_srcRect.h = 500;
	m_dstRect = *_dstRect;
	m_dstRect.w *= 2;
	m_dstRect.h *= 2;
	m_dstRect.x -= m_dstRect.w / 4;
	m_dstRect.y -= m_dstRect.h / 4;
}

void Splat::update(int _xOffset, int _yOffset)
{
	m_dstRect.x -= _xOffset;
	m_dstRect.y -= _yOffset;
}
