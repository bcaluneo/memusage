// Copyright (C) Brendan Caluneo

#include <psapi.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include "SDL.h"
#include <limits>

#ifndef DATA_HH
#define DATA_HH

extern bool quit;

// TODO: Maybe change this so if exe is in the windows directory it counts.
static const std::vector<std::string> systemProcesses {
  "explorer.exe",
  "Explorer.EXE",
  "ctfmon.exe",
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
  "backgroundTaskHost.exe",
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

bool pollSystemList(std::string nameString) {
  for (auto s : systemProcesses) {
      if (nameString.find(s) != std::string::npos) return 1;
  }
  return 0;
}

int getData(void *data) {
  Chart* c = static_cast<Chart*>(data);

  while (!quit) {
    if (!c) continue;

    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);
    c->setPhysicalMemory(memStatus.ullTotalPhys, memStatus.ullTotalPhys-memStatus.ullAvailPhys);

    DWORD processes[MB], cbNeeded, totalProcesses;
    EnumProcesses(processes, sizeof(processes), &cbNeeded);
    totalProcesses = cbNeeded / sizeof(DWORD);

    unsigned systemUsage = 0;
    std::map<std::string, size_t> commonEx;
    for (size_t i = 0; i < totalProcesses; i++) {
      HANDLE hProcess;
      hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ, FALSE, processes[i]);
      if (!hProcess) continue;

      PROCESS_MEMORY_COUNTERS_EX pmc;
      TCHAR baseName[MAX_PATH], modName[MAX_PATH];
      GetModuleBaseNameA(hProcess, nullptr, baseName, sizeof(baseName)/sizeof(*baseName));
      GetModuleFileNameExA(hProcess, nullptr, modName, sizeof(modName)/sizeof(*modName));
      GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*) &pmc, sizeof(pmc));

      if (pmc.WorkingSetSize/(MB*MB) == 0) continue;

      std::string name { baseName }, modNameString { modName };
      if (modNameString.find("C:\\Windows") != std::string::npos || pollSystemList(name)) {
        systemUsage += pmc.WorkingSetSize;
      } else {
        auto it = commonEx.find(name);
        if (it == commonEx.end()) {
          commonEx.insert({name, pmc.WorkingSetSize});
        } else {
          it->second += pmc.WorkingSetSize;
        }
      }

      CloseHandle(hProcess);
    }

    c->setProcess(0, systemUsage, false);
    if (!commonEx.empty()) {
      for (const auto [name, amount] : commonEx) {
        auto ix = c->find(name);
        if (ix == -1) c->addProcess(name, amount);
        else c->setProcess(ix, amount, false);
      }
    }

    SDL_Delay(POLL_TIME);
  }

  return 0;
}

#endif
