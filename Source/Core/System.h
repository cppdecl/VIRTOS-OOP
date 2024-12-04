#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include "Utils/Console.hh"
#include "Utils/Thread.hh"
#include "Utils/IO.hh"
#include "Utils/Clock.hh"
#include "Utils/Random.hh"
#include "Utils/UUID.hh"

#include "Constants/Paths.hh"

class Shell;
class ProgramManager;
class UserAccountControl;

class System 
{
    public:
    System();
    static System& GetInstance();

    bool Startup();
    void Run();
    void Shutdown();
    void Cleanup();

    bool IsRunning() const { return m_Running; }

    Shell* GetShell();
    ProgramManager* GetProgramManager();
    UserAccountControl* GetUserAccountControl();

    private:
    Shell* m_Shell;
    ProgramManager* m_ProgramManager;
    UserAccountControl* m_UserAccountControl;

    bool m_Running = false;
};

#endif // CORE_SYSTEM_H