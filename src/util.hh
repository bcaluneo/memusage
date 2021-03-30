// Copyright (C) Brendan Caluneo

#include <string>
#include <utility>

#ifndef UTIL_HH
#define UTIL_HH

void drawText(SDL_Renderer *render, std::string msg, NFont &font, int x, int y, NFont::Color col) {
	font.draw(render, x, y, col, msg.c_str());
}

void drawTextWithShadow(SDL_Renderer *render, std::string msg, NFont &font, int x, int y, NFont::Color col) {
	font.draw(render, x + 2, y + 2, NFont::Color(0, 0, 0, 255), msg.c_str());
	font.draw(render, x, y, col, msg.c_str());
}

void drawCenterText(SDL_Renderer *render, std::string msg, NFont &font, int x, int y, int w, int h, bool shadow, NFont::Color col) {
	auto txtW = font.getWidth(msg.c_str());
	auto txtH = font.getHeight(msg.c_str());
	if (shadow)	font.draw(render, x+((w-txtW)/2) + 2, y+((h-txtH)/2) + 2, NFont::Color(0,0,0,255), msg.c_str());
	font.draw(render, x+((w-txtW)/2), y+((h-txtH)/2), col, msg.c_str());
}

//src: https://stackoverflow.com/a/24527943
void drawCircle(SDL_Renderer * renderer, int centerX, int centerY, int radius) {
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
