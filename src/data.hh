// Copyright (C) Brendan Caluneo

#include <psapi.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include "SDL.h"

#ifndef DATA_HH
#define DATA_HH

#define bytesToMB(b) b/(1024.0*1024.0)
#define percent(a, b) a*100 / b

typedef unsigned long long ull;
typedef std::vector<std::tuple<std::string, size_t, ull, bool>> LIST;

extern bool quit;

static const std::vector<std::string> systemProcesses {
  "explorer.exe",
  "ntoskrnl.exe",
  "WerFault.exe",
  "backgroundTaskHost.exe",
  "backgroundTransferHost.exe",
  "winlogon.exe",
  "wininit.exe",
  "csrss.exe",
  "lsass.exe",
  "smss.exe",
  "services.exe",
  "taskeng.exe",
  "taskhost.exe",
  "dwm.exe",
  "conhost.exe",
  "svchost.exe",
  "sihost.exe"
};

void PrintMemoryInfo(DWORD processID) {
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


int getData(void *data) {
  Chart* c = static_cast<Chart*>(data);
  while (!quit) {
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);
    c->setPhysicalMemory(bytesToMB(memStatus.ullTotalPhys), bytesToMB(memStatus.ullTotalPhys)-bytesToMB(memStatus.ullAvailPhys));

    DWORD processes[1024], cbNeeded, totalProcesses;
    EnumProcesses(processes, sizeof(processes), &cbNeeded);
    totalProcesses = cbNeeded / sizeof(DWORD);

    for (size_t i = 0; i < totalProcesses; i++) {
      HANDLE hProcess;
      PROCESS_MEMORY_COUNTERS_EX pmc;

      hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processes[i] );
      if (NULL == hProcess) continue;

      TCHAR nameProc[MAX_PATH];
      GetModuleFileNameExA(hProcess, nullptr, nameProc, sizeof(nameProc)/sizeof(*nameProc));
      GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS *) &pmc, sizeof(pmc));

      std::string nameString { nameProc };
      for (auto s : systemProcesses) {
          // if (nameString.find(s) != std::string::npos) {
          //   std::cout << nameString << "\n";
          //   std::cout << "Memory Usage (MB): " << double(bytesToMB(pmc.WorkingSetSize)) << "\n";
          //   std::cout << "\n";
          // }
      }

      auto processInformation = std::make_tuple(nameProc, 0, pmc.WorkingSetSize, 0);
      CloseHandle(hProcess);
    }

    SDL_Delay(1000);
  }

  return 0;
}

#endif
