#include <vector>
#include "GameObject.h"
#include "CombatManager.h"
#include <SDL.h>



void CombatManager::renderEverything(Player * _player, std::vector<Enemy*> _enemy, std::vector<Splat*> _corpse, Background *_borderLR, Background *_borderTB, Background *_bg, Goal *_goal, SDL_Renderer *_renderer)
{
	_bg->updateAndRender(m_permXOffset, m_permYOffset, _renderer, m_winX, m_winY);
	_borderLR->update(0 - m_permXOffset, 0 - m_permYOffset);
	_borderTB->update(0 - m_permXOffset, 0 - m_permYOffset);
	_borderTB->render(_renderer);
	_borderLR->render(_renderer);
	_borderLR->update(m_winX - m_permXOffset, 0 - m_permYOffset);
	_borderTB->update(0 - m_permXOffset + 5, m_winY - m_permYOffset);
	_borderTB->render(_renderer);
	_borderLR->render(_renderer);

	_goal->update(m_permXOffset, m_permYOffset, m_halfX, m_halfY);
	_goal->render(_renderer);

	if (_corpse.size() != 0)
	{
		for (int i = 0; i < _corpse.size(); i++)
		{
			_corpse[i]->update(m_xOffset, m_yOffset);
			_corpse[i]->render(_renderer);
		}
	}

	for (int i = 0; i < _enemy.size(); i++)
	{
		_enemy[i]->update(_enemy[i], _goal, m_xOffset, m_yOffset);
		_enemy[i]->render(_renderer);

	}

	_player->render(_renderer);
}

std::vector<Enemy*> CombatManager::checkAndSpawn(std::vector<Enemy*> _enemy, SDL_Renderer *_renderer, CombatManager *_cm)
{
	while (_enemy.size() <= (_cm->m_score/10) + 10)
	{
		int edge = rand() % 4 + 1;
		float xpos, ypos;
		switch (edge)
		{
			case 1: 
			{
				xpos = 0 - _cm->m_halfX;
				ypos = (rand() % _cm->m_gameX) - _cm->m_halfX;
				break; 
			}

			case 2: 
			{
				ypos = 0 - _cm->m_halfY;
				xpos = (rand() % _cm->m_gameY) - _cm->m_halfY;
				break; 
			}
			case 3: 
			{
				xpos = _cm->m_winX + _cm->m_halfX;
				ypos = (rand() % _cm->m_gameX) - _cm->m_halfX;
				break; 
			}
			
			default: 
			{
				ypos = (_cm->m_winY + _cm->m_halfY);
				xpos = (rand() % _cm->m_gameY) - _cm->m_halfY;
				break; 
			}
		}
		_enemy.push_back(new Enemy("enemy.bmp", _renderer, xpos, ypos));
	}

	return _enemy;
}

void CombatManager::checkSquish(Player * _player, Enemy * _enemy)
{
	SDL_Rect* pBox = _player->getRect();
	SDL_Rect* eBox = _enemy->getRect();
	int pNearX = pBox->x;
	int eNearX = eBox->x;
	int pNearY = pBox->y;
	int eNearY = eBox->y;

	int pFarX = pNearX + pBox->w;
	int eFarX = eNearX + eBox->w;
	int eFarY = eNearY + eBox->h;
	int pFarY = pNearY + pBox->h;

	if (pNearX <= eFarX && pFarX >= eNearX)
	{
		if (pNearY <= eFarY && pFarY >= eNearY)
		{
			_enemy->setDead(true);
			_enemy->setWin(false);
		}
	}

}


CombatManager::CombatManager(int _winW, int _winH)
{
	m_winX = _winW;
	m_winY = _winH;
	m_gameX = m_winX * 2;
	m_gameY = m_winY * 2;
	m_waveNo = 1;
	m_score = 0;
	m_goalHP = 8;
	m_halfX = m_winX / 2;
	m_halfY = m_winY / 2;
}


void CombatManager::reset(std::vector<Enemy*> _enemy, std::vector<Splat*> _corpse)
{
	int size1 = _enemy.size();
	for (int i = 0; i < size1; i++)
	{
		delete _enemy[0];
		_enemy.erase(_enemy.begin());
		
	}

	int size2 = _corpse.size();
	for (int i = 0; i < size2; i++)
	{
		delete _corpse[0];
		_corpse.erase(_corpse.begin());
		
	}
	m_score = 0;
	m_waveNo = 1;
	m_goalHP = 8;
}

