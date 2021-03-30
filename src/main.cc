// Copyright (C) Brendan Caluneo

#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <map>
#include "../inc/NFont.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "util.hh"
#include "chart.hh"
#include "data.hh"

const unsigned SCREEN_WIDTH = 540;
const unsigned SCREEN_HEIGHT = 640;

bool quit = 0;
SDL_Window *window;
SDL_Renderer *render;

int main(int argc, char **args) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

	window = SDL_CreateWindow("memusage", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        SCREEN_WIDTH,SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  SDL_SetRenderDrawColor(render, 180, 180, 180, 255);
  SDL_RenderClear(render);

  Chart chart;
	SDL_CreateThread(getData, "Data Thread", static_cast<void*>(&chart));

  NFont font(render, "resources/font.ttf", 25, NFont::Color(174, 171, 255, 255));

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

    chart.draw(render);

    SDL_RenderPresent(render);
    SDL_Delay(1000 / 60);
  }

	TTF_Quit();
	SDL_Quit();

  return 0;
}
