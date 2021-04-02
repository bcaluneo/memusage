// Copyright (C) Brendan Caluneo

#include "chart.hh"

Chart::Chart() {
  processList.clear();
  addProcess("System", {0, 0});
  colorList["System"] = {255, 255, 255, 255};
}

void Chart::draw(NFont &font, NFont &fontSmall, SDL_Renderer *render, unsigned h) {
  SDL_Rect bar;
  bar.w = 150;
  bar.h = h*.90;
  bar.x = 25;
  bar.y = (h - bar.h) / 2;

  std::stringstream os;
  os << "Physical Memory " << totalMemory/(MB*MB) << " MB ";
  os << "(" << usedMemory/(MB*MB) << " MB used)";

  unsigned msgX = ((SCREEN_WIDTH-font.getWidth(os.str().c_str()))/2);
  unsigned msgY = h - 2*LARGE_PT;
  unsigned yOff = (h - processList.size()*18) / 2 - 2*SMALL_PT;
  unsigned xOff = 150;
  unsigned textX = bar.x + bar.w;

  if (!processList.empty()) {
    auto totalUsage = getTotalUsage();
    unsigned largestWidth = 0, largestUsage = 0;
    for (auto it = processList.begin(); it != processList.end(); ++it) {
      auto ix = std::distance(processList.begin(), it);
      auto color = colorList[std::get<0>(processList[ix])];
      auto pString = std::get<0>(processList[ix]);
      auto pUsage = std::get<1>(processList[ix]).first;
      auto yScale = SMALL_PT*ix;
      if (font.getWidth(pString.c_str()) > largestWidth) largestWidth = font.getWidth(pString.c_str());
      if (pUsage > largestUsage) largestUsage = pUsage;
      drawTextWithOutline(render, pString, fontSmall, textX + xOff, yOff + yScale, 1, NFont::Color(color.r, color.g, color.b));

      SDL_Rect r {bar.x, bar.y, bar.w, bar.h*(pUsage/1000.0/totalUsage)};
      if (ix > 0) {
        for (int k = ix-1; k >= 0; --k) {
          r.y += bar.h*(std::get<1>(processList[k]).first/1000.0/totalUsage);
        }
      }

      SDL_Rect bg {r.x - 1, r.y - 1, r.w + 2, r.h + 2};

      // TODO: Move this into a util function
      SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
      SDL_RenderFillRect(render, &bg);

      SDL_SetRenderDrawColor(render, color.r, color.g, color.b, 255);
      SDL_RenderFillRect(render, &r);
      SDL_RenderDrawLine(render, textX + xOff-5, yOff + yScale + font.getHeight("H")/2, bar.x + bar.w + 1, (r.y + r.h) - r.h/2);
    }

    // TODO: Make this cleaner.
    std::stringstream ls, lss;
    ls << largestUsage;
    lss << "(" << getLargestSub() << ")";
    auto startX = textX + xOff + largestWidth;
    auto off = 0;
    for (unsigned i = 0; i < processList.size(); ++i) {
      std::stringstream os, os2;
      auto color = colorList[std::get<0>(processList[i])];
      auto subAmount = std::get<1>(processList[i]).second;

      os << "(" << subAmount << ")";
      off = font.getWidth(lss.str().c_str()) - font.getWidth(os.str().c_str());
      os << ": "/*<< std::get<1>(processList[i]).first/(MB*MB)*/;
      drawTextWithOutline(render, os.str(), fontSmall, startX + off, yOff + SMALL_PT*i, 1, NFont::Color(color.r, color.g, color.b));

      os.str("");
      os << std::get<1>(processList[i]).first/(MB*MB);
      off = font.getWidth(ls.str().c_str()) - font.getWidth(os.str().c_str());
      drawTextWithOutline(render, os.str(), fontSmall, startX + off, yOff + SMALL_PT*i, 1, NFont::Color(color.r, color.g, color.b));

      os.str("");
      os << "MB";
      off = font.getWidth(lss.str().c_str()) + font.getWidth(ls.str().c_str()) - font.getWidth(os.str().c_str());
      drawTextWithOutline(render, os.str(), fontSmall, startX + off, yOff + SMALL_PT*i, 1, NFont::Color(color.r, color.g, color.b));

      continue;

      for (unsigned j = 0; j < lss.str().size() + ls.str().size() + 1; ++j) {
        os2 << " ";
      }

      auto width0 = font.getWidth(os2.str().c_str());
      drawTextWithOutline(render, " MB", fontSmall, startX + width0, yOff + SMALL_PT*i, 1, NFont::Color(color.r, color.g, color.b));
    }
  }

  SDL_RenderSetScale(render, 1.0, 1.0);
  drawTextWithOutline(render, os.str(), font, msgX, msgY, 1, NFont::Color(255, 255, 255, 255));
  SDL_RenderSetScale(render, RENDER_SCALE, RENDER_SCALE);
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

void Chart::addProcess(std::string name, const std::pair<size_t, size_t>& stats) {
  processList.push_back({name, stats});
  colorList.insert({name, {100+rand()%50, 110+rand()%80, 130+rand()%100, 255}});
}

size_t Chart::getTotalUsage() {
  size_t result = 0;
  for (const auto [ix, stats] : processList) {
    result += stats.first;
  }

  return result/MB;
}

size_t Chart::getLargestSub() {
  size_t result = 0;
  for (const auto [ix, stats] : processList) {
    if (stats.second > result) result = stats.second;
  }

  return result;
}

void Chart::setProcess(signed ix, const std::pair<size_t, size_t>& stats) {
  std::get<1>(processList[ix]).first = stats.first;
  std::get<1>(processList[ix]).second = stats.second;
}

bool Chart::clear() {
  processList.clear();
  // colorList.clear();
  addProcess("System", {0, 0});
  return true;
}

unsigned Chart::getSize() {
  return processList.size();
}
