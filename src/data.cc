// Copyright (C) Brendan Caluneo

#include "data.hh"
#include <cstdlib>

Data::Data() {
  memStatus.dwLength = sizeof(memStatus);
  GlobalMemoryStatusEx(&memStatus);

  totalMemory = memStatus.ullTotalPhys;
  processList = std::make_unique<LIST>();
}

void Data::PrintMemoryInfo(DWORD processID) {
  HANDLE hProcess;
  PROCESS_MEMORY_COUNTERS_EX pmc;

  hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                                  PROCESS_VM_READ,
                                      FALSE, processID );
  if (NULL == hProcess)
    return;

  TCHAR nameProc[MAX_PATH];
  GetModuleFileNameExA(hProcess, nullptr, nameProc, sizeof(nameProc)/sizeof(*nameProc));
  GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS *) &pmc, sizeof(pmc));

  std::cout << nameProc << "\n";
  // This returns information that is technically correct. This line will compute the Memory
  // that is being used by a process that also includes memory that isn't private to that process.
  // Task manager will just give the private memory of a process.
  std::cout << "Memory Usage (MB): " << double(pmc.WorkingSetSize  / (1024.0*1024.0)) << "\n";
  std::cout << "Memory Usage (MB): " << double(pmc.WorkingSetSize  / (1000.0*1000.0)) << "\n";
  std::cout << "\n";

  CloseHandle(hProcess);
}


int Data::getData(void *) {
  while (!quit) {
    processList->clear();
    GlobalMemoryStatusEx(&memStatus);
    auto free = std::make_tuple("free", 0, memStatus.ullAvailPhys, 0);
    processList->push_back(free);

    DWORD processes[MAX_IDENTIFIERS], cbNeeded, totalProcesses;
    EnumProcesses(processes, sizeof(processes), &cbNeeded);
    totalProcesses = cbNeeded / sizeof(DWORD);

    for (size_t i = 0; i < totalProcesses; i++)
    {
      HANDLE hProcess;
      PROCESS_MEMORY_COUNTERS_EX pmc;

      hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                                      PROCESS_VM_READ,
                                          FALSE, processes[i] );
      if (NULL == hProcess) continue;

      TCHAR nameProc[MAX_PATH];
      GetModuleFileNameExA(hProcess, nullptr, nameProc, sizeof(nameProc)/sizeof(*nameProc));
      GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS *) &pmc, sizeof(pmc));

      // std::cout << nameProc << "\n";
      // This returns information that is technically correct. This line will compute the Memory
      // that is being used by a process that also includes memory that isn't private to that process.
      // Task manager will just give the private memory of a process.
      // std::cout << "Memory Usage (MB): " << double(pmc.WorkingSetSize  / (1024.0*1024.0)) << "\n";
      // std::cout << "Memory Usage (MB): " << double(pmc.WorkingSetSize  / (1000.0*1000.0)) << "\n";
      // std::cout << "\n";

      auto processInformation = std::make_tuple(nameProc, 0, pmc.WorkingSetSize, 0);
      // if (processList->size() < 2)
      processList->push_back(processInformation);

      CloseHandle(hProcess);
    }

    draw();
    SDL_Delay(1000);
  }

  return 0;
}

void Data::draw() {
  SDL_Rect bar;

  // TODO: Make these not magic later.
  bar.w = 150;
  bar.h = 512;
  bar.x = 25;
  bar.y = (640 - bar.h) / 2;

  SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
  SDL_RenderFillRect(render, &bar);

  for (auto it = processList->begin(); it != processList->end(); it++) {
    auto pos = std::distance(processList->begin(), it);

    auto process = *it;
    std::string name = std::get<0>(process);
    size_t processCount = std::get<1>(process);
    ull memUsage = std::get<2>(process);
    bool drawn = std::get<3>(process);

    if (drawn) continue;

    auto p = percent(memUsage, totalMemory);

    SDL_Rect processRect;
    processRect.x = 25;
    processRect.w = 150;
    processRect.h = percent(p, 512);
    if (pos == 0) {
      processRect.y = bar.y;
    } else {
      auto prevUsage = std::get<2>(processList->at(pos-1));
      auto prevP = percent(prevUsage, totalMemory);
      processRect.y = 1 + bar.y + percent(prevP, 512);
    }


    std::cout << "Process: " << name << "\n";
    std::cout << "Rect Y: " << processRect.y << "\n";
    std::cout << "Count: " << processCount << "\n";
    std::cout << "Memory Usage: " << bytesToMb(memUsage) << "\n";

    if (pos == 0) {
      SDL_SetRenderDrawColor(render, 0, 255, 0, 0);
    } else {
      SDL_SetRenderDrawColor(render, rand()%255, rand()%255, rand()%255, 0);
    }

    SDL_RenderFillRect(render, &processRect);
    drawn = 1;
  }

}
