#ifndef LISTFILES_PROGRAM_HH
#define LISTFILES_PROGRAM_HH

#include "../../Program.h"

class ListFilesProgram : public Program 
{
    public:
    ListFilesProgram();
    ~ListFilesProgram();
    
    public:
    void Run(std::vector<std::string> args) override;
};

#endif // LISTFILES_PROGRAM_HH