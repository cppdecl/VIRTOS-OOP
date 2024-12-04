#include "MakeDirProgram.h"

#include "Constants/Paths.hh"
#include "Core/System.h"
#include "Core/UserAccountControl/UserAccountControl.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

MakeDirProgram::MakeDirProgram()
{

}

MakeDirProgram::~MakeDirProgram()
{
}

void MakeDirProgram::Run(std::vector<std::string> args)
{
    if (args.size() < 1)
    {
        std::cout << "Usage: mkdir <directory>" << std::endl;
        return;
    }

    std::string directory = args[0];
    System& sys = System::GetInstance();
    UserAccount* user = sys.GetUserAccountControl()->GetCurrentUser();

    directory = user->GetCurrentPath() + directory;

    if (!user->IsRoot())
    {
        if (user->GetCurrentPath().find(user->GetHomePath()) == std::string::npos)
        {
            std::cout << "Permission denied: You can only create folders in your home directory." << std::endl;
            return;
        }
    }

    if (fs::exists(directory))
    {
        std::cout << "Error: Directory already exists." << std::endl;
        return;
    }

    if (fs::create_directories(directory))
    {
        std::cout << "Directory created: " << directory << std::endl;
    }
    else
    {
        std::cout << "Error: Failed to create directory." << std::endl;
    }
}
