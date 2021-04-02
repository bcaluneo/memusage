// Copyright (C) Brendan Caluneo

#include "SDL.h"
#include <windows.h>
#include <tuple>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <iomanip>
#include <cmath>
#include "util.hh"
#include "../inc/NFont.h"

#ifndef CHART_HH
#define CHART_HH

class Chart {
public:
  Chart();
  void draw(NFont &font, NFont &fontSmall, SDL_Renderer *render, unsigned h);
  void setPhysicalMemory(size_t totalMemory, size_t availMemory);
  void addProcess(std::string name, const std::pair<size_t, size_t>& stats);
  void setProcess(signed ix, const std::pair<size_t, size_t>& stats);
  size_t getTotalUsage();
  size_t getLargestSub();
  signed find(const std::string& processName);
  bool clear();
  unsigned getSize();

private:
  std::vector<Process> processList;
  std::map<std::string, SDL_Color> colorList;
  size_t totalMemory, usedMemory;
};

#endif
