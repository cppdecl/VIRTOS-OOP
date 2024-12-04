#ifndef MAKEDIR_PROGRAM_HH
#define MAKEDIR_PROGRAM_HH

#include "../../Program.h"

class MakeDirProgram : public Program 
{
    public:
    MakeDirProgram();
    ~MakeDirProgram();
    
    public:
    void Run(std::vector<std::string> args) override;
};

#endif // MAKEDIR_PROGRAM_HH