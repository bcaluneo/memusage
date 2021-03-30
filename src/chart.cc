// Copyright (C) Brendan Caluneo

#include "chart.hh"
#include <iostream>

#define bytesToMB(b) b/(1024.0*1024.0)
#define percent(a, b) a*100 / b

Chart::Chart() {

}

void Chart::setPhysicalMemory(unsigned totalMemory, unsigned usedMemory) {
  this->totalMemory = totalMemory;
  this->usedMemory = usedMemory;
}

void Chart::draw(SDL_Renderer *render) {
  SDL_Rect bar;

  // TODO: Make these not magic later.
  bar.w = 150;
  bar.h = 512;
  bar.x = 25;
  bar.y = (640 - bar.h) / 2;

  SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
  SDL_RenderFillRect(render, &bar);

  // for (auto it = processList->begin(); it != processList->end(); it++) {
  //   auto pos = std::distance(processList->begin(), it);
  //
  //   auto process = *it;
  //   std::string name = std::get<0>(process);
  //   size_t processCount = std::get<1>(process);
  //   ull memUsage = std::get<2>(process);
  //   bool drawn = std::get<3>(process);
  //
  //   if (drawn) continue;
  //
  //   auto p = percent(memUsage, totalMemory);
  //
  //   SDL_Rect processRect;
  //   processRect.x = 25;
  //   processRect.w = 150;
  //   processRect.h = percent(p, 512);
  //   if (pos == 0) {
  //     processRect.y = bar.y;
  //   } else {
  //     auto prevUsage = std::get<2>(processList->at(pos-1));
  //     auto prevP = percent(prevUsage, totalMemory);
  //     processRect.y = 1 + bar.y + percent(prevP, 512);
  //   }
  //
  //
  //   std::cout << "Process: " << name << "\n";
  //   std::cout << "Rect Y: " << processRect.y << "\n";
  //   std::cout << "Count: " << processCount << "\n";
  //   std::cout << "Memory Usage: " << bytesToMb(memUsage) << "\n";
  //
  //   if (pos == 0) {
  //     SDL_SetRenderDrawColor(render, 0, 255, 0, 0);
  //   } else {
  //     SDL_SetRenderDrawColor(render, rand()%255, rand()%255, rand()%255, 0);
  //   }
  //
  //   SDL_RenderFillRect(render, &processRect);
  //   drawn = 1;
  // }

}
