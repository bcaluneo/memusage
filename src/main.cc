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

bool quit = 0;
SDL_Window *window;
SDL_Renderer *render;

int main(int argc, char **args) {
  SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("memusage", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        SCREEN_WIDTH,SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN /*|SDL_WINDOW_RESIZABLE*/);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  SDL_SetRenderDrawColor(render, 100, 100, 100, 255);
  SDL_RenderClear(render);

  Chart chart;
	SDL_CreateThread(getData, "Data Thread", static_cast<void*>(&chart));

  NFont font(render, "resources/font.ttf", LARGE_PT, NFont::Color(174, 171, 255, 255)),
        fontSmall(render, "resources/font.ttf", SMALL_PT, NFont::Color(174, 171, 255, 255));

  SDL_RenderSetScale(render, RENDER_SCALE, RENDER_SCALE);

  unsigned yoff = 0;
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
          if (k == SDLK_u) yoff-=20;
          // if (k == SDLK_j) yoff-=20;
					break;
				}
		}

    SDL_SetRenderDrawColor(render, 4, 28, 80, 255);
    SDL_RenderClear(render);

    chart.draw(font, fontSmall, render, yoff);

    SDL_RenderPresent(render);
    SDL_Delay(1000);
  }

	SDL_Quit();

  return 0;
}
