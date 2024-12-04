#include "MoveProgram.h"

#include "Constants/Paths.hh"
#include "Core/System.h"
#include "Core/UserAccountControl/UserAccountControl.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

MoveProgram::MoveProgram()
{

}

MoveProgram::~MoveProgram()
{
}

void MoveProgram::Run(std::vector<std::string> args)
{
    if (args.size() < 2)
    {
        std::cout << "Usage: mv <source> <destination>" << std::endl;
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
        std::cout << "Error: Source path does not exist." << std::endl;
        return;
    }

    if (fs::is_directory(destination))
    {
        destination += "/" + fs::path(source).filename().string();
    }

    if (!user->IsRoot())
    {
        if (destination == Constants::UserBaseDir || destination == Constants::OSDir ||
            destination.find(Constants::UserBaseDir) == 0 || destination.find(Constants::OSDir) == 0)
        {
            std::cout << "Permission denied: Cannot move files to restricted directories." << std::endl;
            return;
        }

        if (source.find(Constants::UserBaseDir) == 0 && 
            source.substr(Constants::UserBaseDir.size()).find(user->GetUsername()) == std::string::npos)
        {
            std::cout << "Permission denied: Cannot move from another user's directory." << std::endl;
            return;
        }

        if (destination.find(Constants::UserBaseDir) == 0 && 
            destination.substr(Constants::UserBaseDir.size()).find(user->GetUsername()) == std::string::npos)
        {
            std::cout << "Permission denied: Cannot move to another user's directory." << std::endl;
            return;
        }
    }

    if (fs::exists(destination))
    {
        std::cout << "Error: Destination already exists." << std::endl;
        return;
    }

    try
    {
        fs::rename(source, destination);
        std::cout << "Moved " << source << " to " << destination << "." << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: Failed to move - " << e.what() << std::endl;
    }
}
