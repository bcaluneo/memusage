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

#define bytesToMB(b) b/(1024*1024)
#define percent(a, b) a*100 / b

typedef unsigned long long ull;
typedef std::vector<std::tuple<std::string, size_t, ull, bool>> LIST;

extern bool quit;

static const std::vector<std::string> systemProcesses {
  "explorer.exe",
  "Explorer.EXE",
  "ntoskrnl.exe",
  "TextInputHost.exe",
  "ApplicationFrameHost.exe",
  "DllHost.exe",
  "SecurityHealthSysTray.exe",
  "CompPkgSrv.exe",
  "ShellExperienceHost.exe",
  "UserOOBEBroker.exe",
  "SystemSettingsBroker.exe",
  "AppVShNotify.exe",
  "Microsoft.Photos.exe",
  "WerFault.exe",
  "SearchFilterHost.exe",
  "backgroundTaskHost.exe",
  "backgroundTransferHost.exe",
  "winlogon.exe",
  "taskhostw.exe",
  "StartMenuExperienceHost.exe",
  "wininit.exe",
  "csrss.exe",
  "lsass.exe",
  "smss.exe",
  "services.exe",
  "taskeng.exe",
  "taskhost.exe",
  "SearchApp.exe",
  "SettingSyncHost.exe",
  "dwm.exe",
  "conhost.exe",
  "cmd.exe",
  "svchost.exe",
  "lsm.exe",
  "sihost.exe"
};

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

    unsigned systemUsage = 0;
    std::map<unsigned, unsigned long> commonEx;
    for (size_t i = 0; i < totalProcesses; i++) {
      bool system = 0;
      HANDLE hProcess;
      PROCESS_MEMORY_COUNTERS_EX pmc;

      hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processes[i] );
      if (NULL == hProcess) continue;

      TCHAR nameProc[MAX_PATH];
      GetModuleBaseNameA(hProcess, nullptr, nameProc, sizeof(nameProc)/sizeof(*nameProc));
      GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS *) &pmc, sizeof(pmc));

      std::string nameString { nameProc };
      for (auto s : systemProcesses) {
          if (nameString.find(s) != std::string::npos) {
            system = 1;
            break;
          }
      }

      // auto processInformation = std::make_tuple(nameProc, 0, pmc.WorkingSetSize, 0);
      if (system) {
        systemUsage += bytesToMB(pmc.PrivateUsage);
      } else {
        unsigned ix = c->find(nameString);
        if (ix == -1) c->addProcess({nameString, pmc.WorkingSetSize});
        else {
          auto it = commonEx.find(ix);
          if (it == commonEx.end()) {
            commonEx.insert({ix, pmc.WorkingSetSize});
          } else {
            it->second += pmc.WorkingSetSize;
          }
        }
      }

      CloseHandle(hProcess);
    }

    c->setProcess(0, systemUsage, false);
    if (!commonEx.empty()) {
      for (const auto [ix, amount] : commonEx) {
        c->setProcess(ix, amount, false);
      }
    }

    SDL_Delay(1000);
  }

  return 0;
}

#endif
