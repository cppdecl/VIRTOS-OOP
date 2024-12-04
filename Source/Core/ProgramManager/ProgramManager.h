#ifndef CORE_PROGRAMMANAGER_PROGRAMMANAGER_H
#define CORE_PROGRAMMANAGER_PROGRAMMANAGER_H

#include <unordered_map>
#include <string>

class Program;
class ProgramManager 
{
    public:
    ProgramManager();
    ~ProgramManager();

    void RegisterPrograms();
    void UnregisterPrograms();

    public:
    bool Run(std::string command, std::vector<std::string> args);
    
    private:
    std::unordered_map<std::string, Program*> m_Programs = {};
};

#endif // CORE_PROGRAMMANAGER_PROGRAMMANAGER_H