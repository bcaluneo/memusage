// Copyright (C) Brendan Caluneo

#include "chart.hh"

#define percent(a, b) a*100 / b

Chart::Chart() {
  processList.clear();
  addProcess("System", 0);
}

void Chart::draw(NFont &font, NFont &fontSmall, SDL_Renderer *render, unsigned h) {
  SDL_Rect bar;
  bar.w = 150;
  bar.h = h - (h*.02);
  bar.x = 25;
  bar.y = (h - bar.h) / 2;

  std::stringstream os;
  os << "Physical Memory " << totalMemory/(MB*MB) << " MB ";
  os << "(" << usedMemory/(MB*MB) << " MB used)";
  unsigned msgX = ((SCREEN_WIDTH-font.getWidth(os.str().c_str()))/2);
  unsigned msgY = h - 2*LARGE_PT;
  SDL_RenderSetScale(render, 1.0, 1.0);
  drawTextWithOutline(render, os.str(), font, msgX, msgY, 1, NFont::Color(255, 255, 255, 255));
  SDL_RenderSetScale(render, RENDER_SCALE, RENDER_SCALE);

  unsigned yoff = (h - processList.size()*18) / 2 - 2*SMALL_PT;

  if (!processList.empty()) {
    auto totalUsage = getTotalUsage();
    unsigned largestSize = 0, largestUsage = 0;
    for (auto it = processList.begin(); it != processList.end(); ++it) {
      auto ix = std::distance(processList.begin(), it);
      auto color = colorList[std::get<0>(processList[ix])];
      auto pString = std::get<0>(processList[ix]);
      auto pUsage = std::get<1>(processList[ix]);
      if (pString.size() > largestSize) largestSize = pString.size();
      if (pUsage > largestUsage) largestUsage = pUsage;
      drawTextWithOutline(render, pString, fontSmall, bar.x + bar.w + 30, yoff + SMALL_PT*ix, 1, NFont::Color(color.r, color.g, color.b));

      SDL_Rect r {bar.x, bar.y, bar.w, bar.h*(std::get<1>(processList[ix])/1000.0/totalUsage)};
      if (ix > 0) {
        for (int k = ix-1; k >= 0; --k) {
          r.y += bar.h*(std::get<1>(processList[k])/1000.0/totalUsage);
        }
      }

      SDL_SetRenderDrawColor(render, color.r, color.g, color.b, 255);
      SDL_RenderFillRect(render, &r);
      SDL_RenderDrawLine(render, bar.x + bar.w + 25, yoff + SMALL_PT*ix + font.getHeight("H")/2, bar.x + bar.w + 1, (r.y + r.h) - r.h/2);
    }

    std::stringstream ls;
    ls << largestUsage;
    for (unsigned i = 0; i < processList.size(); ++i) {
      std::stringstream ts, os, os2;
      ts << std::get<1>(processList[i])/(MB*MB);
      auto color = colorList[std::get<0>(processList[i])];
      for (unsigned j = 0; j < largestSize; ++j) {
        os << " ";
        os2 << " ";
      }

      auto width = font.getWidth(os.str().c_str());
      os << ": "<< std::get<1>(processList[i])/(MB*MB);
      drawTextWithOutline(render, os.str(), fontSmall, bar.x + bar.w + width, yoff + SMALL_PT*i, 1, NFont::Color(color.r, color.g, color.b));

      for (unsigned j = 0; j < ls.str().size() + 1; ++j) {
        os2 << " ";
      }

      auto width0 = font.getWidth(os2.str().c_str());
      drawTextWithOutline(render, " MB", fontSmall, bar.x + bar.w + width + width0, yoff + SMALL_PT*i, 1, NFont::Color(color.r, color.g, color.b));
    }
  }
}

void Chart::setPhysicalMemory(size_t totalMemory, size_t usedMemory) {
  this->totalMemory = totalMemory;
  this->usedMemory = usedMemory;
}

signed Chart::find(const std::string& processName) {
  if (processList.empty()) return -1;
  auto it = processList.begin();
  for (;it!=processList.end();++it) {
    if (std::get<0>(*it) == processName) return std::distance(processList.begin(), it);
  }

  return -1;
}

void Chart::addProcess(std::string name, size_t amount) {
  processList.push_back({name, amount});
  colorList.insert({name, {100+rand()%50, 110+rand()%80, 130+rand()%80, 255}});
}

size_t Chart::getTotalUsage() {
  size_t result = 0;
  for (const auto [ix, amt] : processList) {
    result += amt;
  }

  return result/MB;
}

void Chart::setProcess(signed ix, size_t amount, bool add) {
  if (add) std::get<1>(processList[ix]) += amount;
  else std::get<1>(processList[ix]) = amount;
}

bool Chart::clear() {
  processList.clear();
  addProcess("System", 0);
  return true;
}

unsigned Chart::getSize() {
  return processList.size();
}
