#ifndef CLOCK_PROGRAM_HH
#define CLOCK_PROGRAM_HH

#include "../../Program.h"

class ClockProgram : public Program 
{
    public:
    ClockProgram();
    ~ClockProgram();
    
    public:
    void Run(std::vector<std::string> args) override;
};

#endif // CLOCK_PROGRAM_HH