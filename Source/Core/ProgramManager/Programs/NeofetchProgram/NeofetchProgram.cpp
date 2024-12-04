#include "NeofetchProgram.h"

#include "Constants/Paths.hh"
#include "Core/System.h"
#include "Core/UserAccountControl/UserAccountControl.h"
#include "Utils/Clock.hh"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

NeofetchProgram::NeofetchProgram()
{

}

NeofetchProgram::~NeofetchProgram()
{
}

void NeofetchProgram::Run(std::vector<std::string> args)
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);

    std::vector<std::string> sysInfoStrs = {
        "OS: VIRTOS",
        "Processor Cores: " +std:: to_string(sysInfo.dwNumberOfProcessors),
        "Processor Architecture: " +
            std::string((sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) ? "x64" :
                   (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) ? "x86" :
                   "Unknown"),
        "Physical Memory: " + std::to_string(memStatus.ullTotalPhys / (1024 * 1024)) + " MB",
        "Available Memory: " + std::to_string(memStatus.ullAvailPhys / (1024 * 1024)) + " MB",
        "Page File Usage: " + std::to_string(memStatus.ullTotalPageFile / (1024 * 1024)) + " MB",
        "Virtual Memory: " + std::to_string(memStatus.ullTotalVirtual / (1024 * 1024)) + " MB",
        "Available Virtual Memory: " + std::to_string(memStatus.ullAvailVirtual / (1024 * 1024)) + " MB",
        "Page File Status: " +
            std::string((memStatus.ullTotalPageFile > 0) ? "Enabled" : "Disabled"),
        "Uptime: Approx. " + std::to_string(GetTickCount64() / 1000 / 60) + " minutes"
    };

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
    for (std::string str : sysInfoStrs)
    {
        std::cout << str << std::endl;
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}