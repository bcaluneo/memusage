// Copyright (C) Brendan Caluneo

#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <map>
#include "SDL.h"
#include "data.hh"

const size_t MENU_HEIGHT = 20;
const size_t SCREEN_WIDTH = 540;
const size_t SCREEN_HEIGHT = 640;

bool quit = 0;
SDL_Window *window;
SDL_Renderer *render;

int main(int argc, char **args) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

	window = SDL_CreateWindow("memusage", SDL_WINDOWPOS_CENTERED,
														SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
														SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  SDL_SetRenderDrawColor(render, 180, 180, 180, 255);
  SDL_RenderClear(render);

  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_KEYUP:
					auto k = event.key.keysym.sym;
					if (k == SDLK_ESCAPE) quit = 1;
					break;
				}
		}

    SDL_RenderPresent(render);
    SDL_Delay(1000 / 60);
  }

	SDL_Quit();

  return 0;
}
