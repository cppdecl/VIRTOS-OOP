#ifndef MOVE_PROGRAM_HH
#define MOVE_PROGRAM_HH

#include "../../Program.h"

class MoveProgram : public Program 
{
    public:
    MoveProgram();
    ~MoveProgram();
    
    public:
    void Run(std::vector<std::string> args) override;
};

#endif // MOVE_PROGRAM_HH