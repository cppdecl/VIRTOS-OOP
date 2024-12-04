#include "CopyProgram.h"

#include "Constants/Paths.hh"
#include "Core/System.h"
#include "Core/UserAccountControl/UserAccountControl.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

CopyProgram::CopyProgram()
{

}

CopyProgram::~CopyProgram()
{
}

void CopyProgram::Run(std::vector<std::string> args)
{
    if (args.size() < 2)
    {
        std::cout << "Usage: cp <source> <destination>" << std::endl;
        return;
    }

    std::string source = args[0];
    std::string destination = args[1];
    System& sys = System::GetInstance();
    UserAccount* user = sys.GetUserAccountControl()->GetCurrentUser();

    source = user->GetCurrentPath() + source;
    destination = user->GetCurrentPath() + destination;

    if (!fs::exists(source))
    {
        std::cout << "Source path does not exist." << std::endl;
        return;
    }

    if (!user->IsRoot())
    {
        if (source.find(Constants::UserBaseDir) == 0 && 
            source.substr(Constants::UserBaseDir.size()).find(user->GetUsername()) == std::string::npos)
        {
            std::cout << "Permission denied: Cannot copy from another user's directory." << std::endl;
            return;
        }

        if (destination.find(Constants::UserBaseDir) == 0 && 
            destination.substr(Constants::UserBaseDir.size()).find(user->GetUsername()) == std::string::npos)
        {
            std::cout << "Permission denied: Cannot copy to another user's directory." << std::endl;
            return;
        }

        if (destination == Constants::UserBaseDir || destination == Constants::OSDir ||
            destination.find(Constants::UserBaseDir) == 0 || destination.find(Constants::OSDir) == 0)
        {
            std::cout << "Permission denied: Cannot copy into restricted directories." << std::endl;
            return;
        }
    }

    if (fs::is_directory(source))
    {
        if (!fs::exists(destination))
        {
            fs::create_directories(destination);
        }

        fs::copy(source, destination, fs::copy_options::recursive);
        std::cout << "Copied directory " << source << " to " << destination << std::endl;
    }
    else
    {
        if (!fs::exists(destination))
        {
            fs::create_directories(destination);
        }

        fs::copy(source, destination + "/" + fs::path(source).filename().string());
        std::cout << "Copied file " << source << " to " << destination << std::endl;
    }
}