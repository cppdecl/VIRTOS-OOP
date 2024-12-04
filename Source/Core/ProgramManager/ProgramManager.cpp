#include "ProgramManager.h"

#include "Programs/ListFilesProgram/ListFilesProgram.h"
#include "Programs/ChangeDirProgram/ChangeDirProgram.h"
#include "Programs/CopyProgram/CopyProgram.h"
#include "Programs/MakeDirProgram/MakeDirProgram.h"
#include "Programs/MoveProgram/MoveProgram.h"
#include "Programs/RemoveProgram/RemoveProgram.h"
#include "Programs/NanoProgram/NanoProgram.h"
#include "Programs/SnakeProgram/SnakeProgram.h"
#include "Programs/ClockProgram/ClockProgram.h"
#include "Programs/NeofetchProgram/NeofetchProgram.h"


ProgramManager::ProgramManager()
{

}

ProgramManager::~ProgramManager()
{

}

bool ProgramManager::Run(std::string command, std::vector<std::string> args)
{
    auto it = m_Programs.find(command);
    if (it != m_Programs.end())
    {
        Program* program = it->second;
        program->Run(args);
        return true;
    }

    return false;
}

void ProgramManager::RegisterPrograms()
{
    m_Programs["listfiles"] = new ListFilesProgram();
    m_Programs["ls"] = m_Programs["listfiles"];

    m_Programs["changedir"] = new ChangeDirProgram();
    m_Programs["cd"] = m_Programs["changedir"];

    m_Programs["copy"] = new CopyProgram();
    m_Programs["cp"] = m_Programs["copy"];

    m_Programs["makedir"] = new MakeDirProgram();
    m_Programs["mkdir"] = m_Programs["makedir"];

    m_Programs["move"] = new MoveProgram();
    m_Programs["mv"] = m_Programs["move"];

    m_Programs["remove"] = new RemoveProgram();
    m_Programs["rm"] = m_Programs["remove"];

    m_Programs["nano"] = new NanoProgram();

    m_Programs["snake"] = new SnakeProgram();
    m_Programs["sss"] = m_Programs["snake"];

    m_Programs["clock"] = new ClockProgram();
    m_Programs["date"] = m_Programs["clock"];
    m_Programs["dt"] = m_Programs["clock"];
    m_Programs["time"] = m_Programs["clock"];

    m_Programs["neofetch"] = new NeofetchProgram();
}

void ProgramManager::UnregisterPrograms()
{
    for (auto it = m_Programs.begin(); it != m_Programs.end(); it++)
    {
        delete it->second;
    }

    m_Programs.clear();
}