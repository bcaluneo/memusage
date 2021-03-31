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

// #define __TEST

bool quit = 0;
SDL_Window *window;
SDL_Renderer *render;
Chart chart;

int renderThread(void *) {
  NFont font(render, "resources/font.ttf", LARGE_PT, NFont::Color(174, 171, 255, 255)),
        fontSmall(render, "resources/font.ttf", SMALL_PT, NFont::Color(174, 171, 255, 255));

  int w, h;
  while (!quit) {
    SDL_SetRenderDrawColor(render, 4, 28, 80, 255);
    SDL_RenderClear(render);

    SDL_GetWindowSize(window, &w, &h);
    chart.draw(font, fontSmall, render, h);

    SDL_RenderPresent(render);
    SDL_Delay(1000);
  }
}

int main(int argc, char **args) {
  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("memusage", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        SCREEN_WIDTH,SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN |SDL_WINDOW_RESIZABLE);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  SDL_SetRenderDrawColor(render, 100, 100, 100, 255);
  SDL_RenderClear(render);

  SDL_CreateThread(renderThread, "Render Thread", nullptr);
  SDL_CreateThread(getData, "Data Thread", static_cast<void*>(&chart));

  SDL_RenderSetScale(render, RENDER_SCALE, RENDER_SCALE);

  SDL_Event event;
  #ifdef __TEST
    size_t* t;
    size_t sz = 4900000000;
  #endif

  while (!quit) {
    while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_KEYUP:
					auto k = event.key.keysym.sym;
					if (k == SDLK_ESCAPE) quit = 1;

          #ifdef __TEST
            if (k == SDLK_h) {
              std::cout << "Allocating t... ";
              t = (size_t*) malloc(sz);
              memset(t, 0, sz);
              std::cout << " OK\n";
            }
            if (k == SDLK_j) {
              std::cout << "Freeing t... ";
              free(t);
              t = nullptr;
              std::cout << " OK\n";
            }
          #endif

          break;
				}
		}

    SDL_Delay(1000/60);
  }

  #ifdef __TEST
    /*Just in case I forgot to free this... */
    if (t) free(t);
  #endif

	SDL_Quit();

  return 0;
}
