// Copyright (C) Brendan Caluneo

#include <windows.h>
#include <psapi.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "SDL.h"

#ifndef DATA_HH
#define DATA_HH

#define bytesToMb(b) b/(1024.0*1024.0)
#define percent(a, b) a*100 / b

typedef unsigned long long ull;
typedef std::vector<std::tuple<std::string, size_t, ull, bool>> LIST;

extern bool quit;
extern SDL_Renderer *render;
const size_t MAX_IDENTIFIERS = 1024;

class Data {
public:
  Data();
  int getData(void *);
  void draw();
  void PrintMemoryInfo(DWORD);

private:
  std::unique_ptr<LIST> processList;
  DWORDLONG totalMemory;
  MEMORYSTATUSEX memStatus;
};

#endif
