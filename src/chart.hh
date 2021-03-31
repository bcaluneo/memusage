// Copyright (C) Brendan Caluneo

#include "SDL.h"
#include <windows.h>
#include <tuple>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include "util.hh"
#include "../inc/NFont.h"

#ifndef CHART_HH
#define CHART_HH

class Chart {
public:
  Chart();
  void draw(NFont &font, SDL_Renderer *render, unsigned yoff);
  void setPhysicalMemory(unsigned totalMemory, unsigned availMemory);
  void clearProcessUsage();
  void addProcess(Process process);
  void setProcess(signed ix, unsigned long amount, bool add);
  unsigned getProcessUsage(signed ix);
  signed find(const std::string& processName);

private:
  std::vector<Process> processList;
  unsigned totalMemory, usedMemory;
};

#endif
