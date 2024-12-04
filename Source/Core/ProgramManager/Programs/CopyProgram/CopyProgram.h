#ifndef COPY_PROGRAM_HH
#define COPY_PROGRAM_HH

#include "../../Program.h"

class CopyProgram : public Program 
{
    public:
    CopyProgram();
    ~CopyProgram();
    
    public:
    void Run(std::vector<std::string> args) override;
};

#endif // COPY_PROGRAM_HH