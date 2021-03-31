// Copyright (C) Brendan Caluneo

#include "chart.hh"

#define percent(a, b) a*100 / b

Chart::Chart() {
  processList.clear();
  addProcess({"System", 0});
}

void Chart::draw(NFont &font, SDL_Renderer *render, unsigned yoff) {
  SDL_Rect bar;

  // TODO: Make these not magic later.
  bar.w = 150;
  bar.h = 800;
  bar.x = 25;
  bar.y = (SCREEN_HEIGHT - bar.h) / 2;

  SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
  SDL_RenderFillRect(render, &bar);

  std::stringstream os;
  os << "Physical Memory " << totalMemory << " MB ";
  os << "(" << usedMemory << " MB used)";
  unsigned msgX = ((SCREEN_WIDTH-font.getWidth(os.str().c_str()))/2);
  unsigned msgY = SCREEN_HEIGHT - 2*20;
  drawTextWithShadow(render, os.str(), font, msgX, msgY, NFont::Color(255, 255, 255, 255));

  if (!processList.empty()) {
    for (auto it = processList.begin(); it != processList.end(); ++it) {
      auto ix = std::distance(processList.begin(), it);
      std::stringstream os;
      os << std::get<0>(processList[ix]) << " : " << std::get<1>(processList[ix])/1024 << " K";
      drawTextWithShadow(render, os.str(), font, bar.x + bar.w + 2*20, yoff + 5+20*ix, NFont::Color(255, 255, 255, 255));
    }
  }
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

void Chart::setPhysicalMemory(unsigned totalMemory, unsigned usedMemory) {
  this->totalMemory = totalMemory;
  this->usedMemory = usedMemory;
}

signed Chart::find(const std::string& processName) {
  auto it = processList.begin();
  for (;it!=processList.end();++it) {
    if (std::get<0>(*it) == processName) return std::distance(processList.begin(), it);
  }

  return -1;
}

void Chart::clearProcessUsage() {
  processList.clear();
  processList.push_back({"System", 0});
}

void Chart::addProcess(Process process) {
  processList.push_back(process);
}

unsigned Chart::getProcessUsage(signed ix) {
  return std::get<1>(processList[ix]);
}

void Chart::setProcess(signed ix, unsigned long amount, bool add) {
  if (add) std::get<1>(processList[ix]) += amount;
  else std::get<1>(processList[ix]) = amount;
}
