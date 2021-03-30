// Copyright (C) Brendan Caluneo

#include "SDL.h"
#include <windows.h>

#ifndef CHART_HH
#define CHART_HH

class Chart {
public:
  Chart();
  void draw(SDL_Renderer *render);
  void setPhysicalMemory(unsigned totalMemory, unsigned availMemory);

private:
  unsigned totalMemory, usedMemory;
};

#endif
