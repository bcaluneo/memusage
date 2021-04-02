// Copyright (C) Brendan Caluneo

#include <string>
#include <utility>
#include <iostream>
#include "../inc/NFont.h"

#ifndef UTIL_HH
#define UTIL_HH

const unsigned SCREEN_WIDTH = 800;
const unsigned SCREEN_HEIGHT = 650;
const unsigned SMALL_PT = 19;
const unsigned LARGE_PT = 20;
const unsigned POLL_TIME = 1000;
const unsigned MB = 1024;
const unsigned CUTOFF = 50;

typedef std::tuple<std::string, std::pair<size_t, size_t>> Process;

namespace Mouse {
	inline unsigned mouseX, mouseY;

	inline void setMousePos(unsigned mX, unsigned mY) {
		mouseX = mX;
		mouseY = mY;
	}
}

inline void drawText(SDL_Renderer *render, std::string msg, NFont &font, int x, int y, NFont::Color col) {
	font.draw(render, x, y, col, msg.c_str());
}

inline void drawTextWithShadow(SDL_Renderer *render, std::string msg, NFont &font, int x, int y, NFont::Color col) {
	for (unsigned i = 0; i < 2; ++i) font.draw(render, x + i, y + i, NFont::Color(0, 0, 0, 255), msg.c_str());
	font.draw(render, x, y, col, msg.c_str());
}

inline void drawTextWithOutline(SDL_Renderer *render, std::string msg, NFont &font, int x, int y, int width, NFont::Color col) {
	for (signed i = -width; i <= width; ++i) {
		for (signed j = -width; j <= width; ++j) {
			font.draw(render, x + i, y + j, NFont::Color(0, 0, 0, 255), msg.c_str());
		}
	}

	font.draw(render, x, y, col, msg.c_str());
}

inline void drawCenterText(SDL_Renderer *render, std::string msg, NFont &font, int x, int y, int w, int h, bool shadow, NFont::Color col) {
	auto txtW = font.getWidth(msg.c_str());
	auto txtH = font.getHeight(msg.c_str());
	if (shadow)	font.draw(render, x+((w-txtW)/2) + 2, y+((h-txtH)/2) + 2, NFont::Color(0,0,0,255), msg.c_str());
	font.draw(render, x+((w-txtW)/2), y+((h-txtH)/2), col, msg.c_str());
}

//src: https://stackoverflow.com/a/24527943
inline void drawCircle(SDL_Renderer * renderer, int centerX, int centerY, int radius) {
   int x = radius;
   int y = 0;
   int error = 1 - x;

   while (x >= y) {
		 int startX = -x + centerX;
		 int endX = x + centerX;
		 SDL_RenderDrawLine(renderer, startX, y + centerY, endX, y + centerY);
		 if (y != 0) {
			 SDL_RenderDrawLine(renderer, startX, -y + centerY, endX, -y + centerY);
		 }

		 y++;

		 if (error<0) {
			 error += 2 * y + 1;
		 } else {
			 if (x >= y) {
				 startX = -y + 1 + centerX;
				 endX = y - 1 + centerX;
				 SDL_RenderDrawLine(renderer, startX, x + centerY, endX,  x + centerY);
				 SDL_RenderDrawLine(renderer, startX, -x + centerY, endX, -x + centerY);
			 }

			 x--;
			 error += 2 * (y - x + 1);
		 }
   }
}

#endif
