#include "System.h"

#include "Shell/Shell.h"
#include "ProgramManager/ProgramManager.h"
#include "UserAccountControl/UserAccountControl.h"

System::System() 
{
    
}

System& System::GetInstance() 
{
    static System instance_;
    return instance_;
}

bool System::Startup() 
{
    Console::Init();

    Console::Log("{}VIRTOS v1.0", Console::Color::GREEN);

    Console::PrintToCurrentLine("{}Initializing Boot Sequence", Console::Color::GREEN);
    for (int i = 0; i < 3; i++)
    {
        Thread::Sleep(500);
        Console::PrintToCurrentLine("{}.", Console::Color::GREEN);
    }
    Console::NextLine();

    m_Shell = new Shell();
    
    m_ProgramManager = new ProgramManager();
    m_ProgramManager->RegisterPrograms();

    m_UserAccountControl = new UserAccountControl();
    m_UserAccountControl->Startup();

    m_Running = true;
    return true;
}

void System::Run() 
{
    if (!GetUserAccountControl()->IsLoggedIn()) 
    {
        GetUserAccountControl()->WaitForLogin();
    }

    GetShell()->Run();
}

void System::Shutdown() 
{
    Console::HideCursor();
    Console::Log("{}VirtOS is shutting down...", Console::Color::MAGENTA);
    Thread::Sleep(2000);

    GetUserAccountControl()->Shutdown();
    GetProgramManager()->UnregisterPrograms();
    
    m_Running = false;
}

void System::Cleanup() 
{
    delete m_Shell;
    delete m_ProgramManager;
    delete m_UserAccountControl;
}

Shell* System::GetShell() 
{
    return m_Shell;
}

ProgramManager* System::GetProgramManager() 
{
    return m_ProgramManager;
}

UserAccountControl* System::GetUserAccountControl() 
{
    return m_UserAccountControl;
}