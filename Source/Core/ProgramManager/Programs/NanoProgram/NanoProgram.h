#ifndef NANO_PROGRAM_HH
#define NANO_PROGRAM_HH

#include "../../Program.h"
#include <string>
#include <vector>

class NanoProgram : public Program 
{
    public:
    NanoProgram();
    ~NanoProgram();

    public:
    void Run(std::vector<std::string> args) override;
};

#endif // NANO_PROGRAM_HH
