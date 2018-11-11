#pragma once
#ifndef _SDL_DISPLAY_H_
#define _SDL_DISPLAY_H_
#include <SDL_mixer.h>

bool loop(bool &_squish, int _hp);
void cleanUp(SDL_Texture **_tex, SDL_Renderer **_renderer, SDL_Window **_window, Mix_Music *_music);



#endif
