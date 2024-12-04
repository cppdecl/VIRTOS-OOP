#ifndef REMOVE_PROGRAM_HH
#define REMOVE_PROGRAM_HH

#include "../../Program.h"

class RemoveProgram : public Program 
{
    public:
    RemoveProgram();
    ~RemoveProgram();
    
    public:
    void Run(std::vector<std::string> args) override;
};

#endif // REMOVE_PROGRAM_HH