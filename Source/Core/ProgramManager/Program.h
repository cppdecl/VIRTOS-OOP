#ifndef CORE_PROGRAMMANAGER_PROGRAM_H
#define CORE_PROGRAMMANAGER_PROGRAM_H

#include <vector>
#include <string>

class Program 
{
    public:
    Program() = default;
    virtual ~Program() = default;

    public:
    virtual void Run(std::vector<std::string> args) = 0;
};

#endif // CORE_PROGRAMMANAGER_PROGRAM_H