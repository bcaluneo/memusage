// Copyright (C) Brendan Caluneo

#include "SDL.h"
#include <windows.h>
#include <tuple>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include "util.hh"
#include "../inc/NFont.h"

#ifndef CHART_HH
#define CHART_HH

class Chart {
public:
  Chart();
  void draw(NFont &font, NFont &fontSmall, SDL_Renderer *render, unsigned yoff);
  void setPhysicalMemory(unsigned long long totalMemory, unsigned long long availMemory);
  void addProcess(std::string name, unsigned long amount);
  void setProcess(signed ix, unsigned long amount, bool add);
  unsigned long getTotalUsage();
  signed find(const std::string& processName);
  bool clear();
  unsigned getSize();

private:
  std::vector<Process> processList;
  std::map<std::string, SDL_Color> colorList;
  unsigned long long totalMemory, usedMemory;
};

#endif
