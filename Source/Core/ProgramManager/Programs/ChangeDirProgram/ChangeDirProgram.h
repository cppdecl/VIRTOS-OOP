#ifndef CHANGEDIR_PROGRAM_HH
#define CHANGEDIR_PROGRAM_HH

#include "../../Program.h"

class ChangeDirProgram : public Program 
{
    public:
    ChangeDirProgram();
    ~ChangeDirProgram();
    
    public:
    void Run(std::vector<std::string> args) override;
};

#endif // CHANGEDIR_PROGRAM_HH