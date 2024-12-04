#include "ClockProgram.h"

#include "Constants/Paths.hh"
#include "Core/System.h"
#include "Core/UserAccountControl/UserAccountControl.h"
#include "Utils/Clock.hh"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

ClockProgram::ClockProgram()
{

}

ClockProgram::~ClockProgram()
{
}

void ClockProgram::Run(std::vector<std::string> args)
{
    std::cout << "Date/Time: " << Clock::GetTimestamp() << std::endl;
}