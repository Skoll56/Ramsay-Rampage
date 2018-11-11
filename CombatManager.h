#pragma once
#ifndef _COMBATMANAGER_H_
#define _COMBATMANAGER_H_


class CombatManager

{	
private:	
	int m_goalHP;
	int m_score;
	
	float m_waveNo;

public:
	CombatManager(int _winW, int _winH);
	~CombatManager() {};	
	int m_gameX;
	int m_gameY;
	int m_winX;
	int m_winY;
	int m_halfY;
	int m_halfX;
	int m_xOffset, m_yOffset, m_permYOffset, m_permXOffset;
	void reset(std::vector<Enemy*> _enemy, std::vector<Splat*> _corpse);
	void setWave(int _wave) { m_waveNo = _wave; }
	int getWave() { return m_waveNo; }
	void addScore(int _scoreBonus) { m_score += _scoreBonus; }
	int getScore() { return m_score; }
	int getGoal() { return m_goalHP; }
	void dmgGoal(int _dmg) { m_goalHP -= _dmg; }
	void renderEverything(Player *_player, std::vector<Enemy*> _enemy, std::vector<Splat*> _corpse, Background *_borderLR, Background *_borderTB, Background *_bg, Goal *_goal, SDL_Renderer *_renderer);
	

	static std::vector<Enemy*> checkAndSpawn(std::vector<Enemy*> _enemy, SDL_Renderer *_renderer, CombatManager *_cm);
	void checkSquish(Player *_player, Enemy *_enemy);

};






#endif
