#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "GameObject.h"
#include "SDL Display.h"
#include "CombatManager.h"
#include <SDL_mixer.h>
#include <time.h>


// Step 1: Controls [X]
//Step 2: Enemy Manager [X]
// Step 3: Enemy AI [X]
//Step 4: Collision Detection[X]
//Step 5: Graphics [X]
//Step 6: Scoring [X]
//Step 7: Death marks [X]
//Step 8: Sound Effects + Music [X]
//Step 9: Main Menu [X] 
//Step 10: Tidy everything [X]
//Step 11: Comment stuff[X]
//Step 12: Remove all these steps [ ] ... 

int main(int argc, char *args[] )

{
	srand(time(NULL));

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Oopsie poopsie" << std::endl;
	}

	if (TTF_Init() < 0)
	{
		std::cout << "Uh oh. TTF DED";
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "SDL Mixer ded fam: " << Mix_GetError();
	}  // All sound code was adapted from Lazy Foo Tutorials

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *tex = nullptr;
	SDL_Surface *surf = nullptr;	
	Uint32 frameStart;	
	Player *player = nullptr;
	Background *bg = nullptr;
	GameObject *mouse = nullptr;
	Goal *goal = nullptr;
	Background *borderTB = nullptr;
	Background *borderLR = nullptr;
	Text *score = nullptr;
	Text *waveNo = nullptr;
	Text *lambHP = nullptr;
	Text *loseMessage = nullptr;
	CombatManager *cm = nullptr;
	Background *instructions = nullptr;
	Background *startEnd = nullptr;
	Background *title = nullptr;
	Mix_Chunk *wehoo = Mix_LoadWAV("Sound/Wehoo.wav");
	Mix_Chunk *whack = Mix_LoadWAV("Sound/Bwrakh.wav");
	Mix_Music *music = Mix_LoadMUS("Sound/Shenanigans.wav");
	window = SDL_CreateWindow("Game" , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, 0);
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	renderer = SDL_CreateRenderer(window, -1, 0);
	int winW, winH;
	SDL_GetWindowSize(window, &winW, &winH);
	cm = new CombatManager(winW, winH);
	player = new Player("Sprite.bmp", renderer, winW, winH);
	goal = new Goal("goal.bmp", renderer, cm->m_halfX, cm->m_halfY);
	bg = new Background("background.bmp", renderer, 0, 0, 500, 500);
	borderTB = new Background("borderh.bmp", renderer, 0, winW);
	borderLR = new Background("border.bmp", renderer, 0, winH);
	std::string scoreString;
	std::string waveString;
	std::string lambString;
	std::string endString;
	SDL_Color red = SDL_Colour{ 255,0,0 };
	SDL_Color blue = SDL_Colour{ 0,0,255 };
	SDL_Color green = SDL_Colour{ 0,255,0 };
	SDL_Color black = SDL_Colour{ 0,0,0 };
	SDL_Color white = SDL_Colour{ 255,255,255 };
	score = new Text(renderer, 0, winH - 50, 150, 50, 40, green, " ");
	waveNo = new Text(renderer, winW - 150, 0, 150, 50, 40, blue, " ");
	lambHP = new Text(renderer, cm->m_halfX - 150, winH - 50, 300, 50, 40, green, " ");
	instructions = new Background("Instructions.bmp", renderer, 0, winH/2-250, 500, 500);
	startEnd = new Background("Start and Quit.bmp", renderer, winW - 500, cm->m_halfY - 250, 500, 500);
	title = new Background("title.bmp", renderer, cm->m_halfX - 250, winH*0.2, 100, 600);
	mouse = new GameObject;
	SDL_ShowCursor(0);

	float frameTime = 0.0f;
	int frameRate = 60;
	float delay = 1000.0f / frameRate;
	bool play = false;
	bool quit = false;

	
	while (quit == false) // Main Menu Loop. If the user quits, the game ends.
	{
		bg->updateAndRender(0, 0, renderer, winW, winH); // Prepare the background for main menu
		title->render(renderer);
		startEnd->render(renderer);
		instructions->render(renderer);
		player->render(renderer);
		SDL_RenderPresent(renderer);
		Mix_PlayMusic(music, -1);
		
		int mouseX = mouse->getX(); // Prototyping a mouse position for GetMouse to use
		int mouseY = mouse->getX();			
		bool squish = false;
		int lastX, lastY;
		int lastScore; 
		int lastWave; // Variables to keep track of the previous score(s). This is so that text only updates if there is a change.
		int lastHP;
		if (play == true) // If the user has pressed space to start the game
		{
			lastX = cm->m_halfX;
			lastY = cm->m_halfY;
			lastScore = 2;
			lastWave = 5; // Random values to start with so the first run will register a change and display the real scores (base)
			lastHP = 0;
			std::vector<Enemy*> enemy;
			std::vector<Splat*> corpse;

			while (loop(squish, cm->getGoal())) //Game loop runs until user presses escape or reaches 0 HP
			{
				frameStart = SDL_GetTicks();
				SDL_GetMouseState(&mouseX, &mouseY); 
				mouse->setX(mouseX);
				mouse->setY(mouseY);
				cm->m_xOffset = mouseX - lastX; // The Offset variables are dynamic variables that track how far...
				cm->m_yOffset = mouseY - lastY; // ...the mouse moved since last frame

				cm->m_permYOffset = mouseY - cm->m_halfY; //The perm offset track the mouse's absolute distance from the ...
				cm->m_permXOffset = mouseX - cm->m_halfX; // ... Middle of the screen, rather than relative to last frame. 				

				enemy = cm->checkAndSpawn(enemy, renderer, cm); // Fills the vector of enemies with a number relative to the score

				cm->renderEverything(player, enemy, corpse, borderLR, borderTB, bg, goal, renderer);
				//Does what it says on the tin. Updates and renders all non-text.

				for (int i = 0; i < enemy.size(); i++) // Check all enemies
				{
					if (squish == true) // If the player has clicked the mouse
					{
						cm->checkSquish(player, enemy[i]); //Check if the player is over an enemy
					}

					if (enemy[i]->getDead() == true)
					{
						if (enemy[i]->getWin() == true)
						{
							Mix_PlayChannel(-1, wehoo, 0);
							cm->dmgGoal(1); // If the enemy won, damage the goal
						}
						else
						{
							Mix_PlayChannel(-1, whack, 0);
							cm->addScore(1); // If the player killed the enemy, add to the score
							corpse.push_back(new Splat(enemy[i]->getRect(), renderer));
							
							if (corpse.size() >= 10) //Only allow 10 Splats on the screen, to save memory
							{
								delete corpse[0];
								corpse.erase(corpse.begin());
							}
						}
						delete enemy[i];
						enemy.erase(enemy.begin() + i);
					}

				}

				cm->setWave((cm->getScore() / 10) + 1);
				
				if (cm->getScore() != lastScore) // Test to see if it needs to update at all (Saves a lot of memory)
				{
					scoreString = "Score: " + std::to_string(cm->getScore());
					score->update(&scoreString, renderer);
				}
				score->renderText(renderer);
				
				if (cm->getWave() != lastWave)
				{
					waveString = "Wave: " + std::to_string(cm->getWave());
					waveNo->update(&waveString, renderer);
				}
				waveNo->renderText(renderer);
						
				
				if (cm->getGoal() != lastHP)
				{
					lambString = "Sauce Dollops: " + std::to_string(cm->getGoal());
					lambHP->update(&lambString, renderer);
				}
				lambHP->renderText(renderer);
				
				//Leave all rendering and updating before this line

				SDL_RenderPresent(renderer);

				//Concluding the game loop and re-setting needed values for the next frame run
				squish = false;
				lastX = mouseX;
				lastY = mouseY;
				lastScore = cm->getScore();
				lastWave = cm->getWave();
				lastHP = cm->getGoal();
				frameTime = SDL_GetTicks() - frameStart;
				if (delay > frameTime) //The frame control code has been adapted from a "Let's make games" Youtube tutorial
				{
					SDL_Delay(delay - frameTime);
				}
			}
			
			play = false;
			endString = "Final Score: " + std::to_string(cm->getScore());
			loseMessage = new Text(renderer, cm->m_halfX - 500, cm->m_halfY - 100, 1000, 200, 200, blue, endString);
			loseMessage->renderText(renderer);
			cm->reset(enemy, corpse); // A mini-clean up that re-sets all the pointers and values for if they start the game again
			SDL_RenderPresent(renderer);
			SDL_Delay(8000);
		}
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT: { quit = true;	break; }
				case SDL_KEYDOWN: 
				{
					if (event.key.keysym.sym == SDLK_ESCAPE) { quit = true; break; }
					if (event.key.keysym.sym == SDLK_SPACE) { play = true; break; }
				}
				default: { break; }
			}
		}
			
	}

	cleanUp(&tex, &renderer, &window, music);
	Mix_Quit();
	TTF_Quit();
	return 0;
} 









void cleanUp(SDL_Texture **_tex, SDL_Renderer **_renderer, SDL_Window **_window, Mix_Music *_music)
{
	SDL_DestroyTexture(*_tex);
	SDL_DestroyRenderer(*_renderer);
	SDL_DestroyWindow(*_window);
	Mix_FreeMusic(_music);
	_music = nullptr;
	SDL_Quit();
}

bool loop(bool &_squish, int _hp)
{
	if (_hp <= 0)
	{
		return false;
	} 
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: { return false;	break; }
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) { return false; break; }
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) 
			{ 
				_squish = true;
			}
		default: { return true;	break; }
		}
	}
	return true;
}