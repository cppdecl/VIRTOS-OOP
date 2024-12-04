#include "RemoveProgram.h"

#include "Constants/Paths.hh"
#include "Core/System.h"
#include "Core/UserAccountControl/UserAccountControl.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

RemoveProgram::RemoveProgram()
{

}

RemoveProgram::~RemoveProgram()
{
}

void RemoveProgram::Run(std::vector<std::string> args)
{
    if (args.size() < 1)
    {
        std::cout << "Usage: rm <path> [-r]" << std::endl;
        return;
    }

    bool forceDelete = false;
    std::string path = args[0];
    
    if (args.size() > 1 && args[0] == "-r")
    {
        forceDelete = true;
    }

    System& sys = System::GetInstance();
    UserAccount* user = sys.GetUserAccountControl()->GetCurrentUser();
    
    if (forceDelete)
    {
        path = args[1];
    }

    path = user->GetCurrentPath() + path;

    if (!fs::exists(path))
    {
        std::cout << "Error: Path does not exist." << std::endl;
        return;
    }

    if (!user->IsRoot())
    {
        if (path.find(Constants::UserBaseDir) == 0 && 
            path.substr(Constants::UserBaseDir.size()).find(user->GetUsername()) == std::string::npos)
        {
            std::cout << "Permission denied: Cannot remove files from another user's directory." << std::endl;
            return;
        }

        if (path == Constants::UserBaseDir || path == Constants::OSDir ||
            path.find(Constants::UserBaseDir) == 0 || path.find(Constants::OSDir) == 0)
        {
            std::cout << "Permission denied: Cannot remove files in restricted directories." << std::endl;
            return;
        }
    }

    if (fs::is_directory(path))
    {
        if (forceDelete)
        {
            try
            {
                fs::remove_all(path); 
                std::cout << "Force removed directory: " << path << std::endl;
            }
            catch (const std::exception& e)
            {
                std::cout << "Error: Failed to force remove directory - " << e.what() << std::endl;
            }
        }
        else if (fs::is_empty(path))
        {
            try
            {
                fs::remove(path);
                std::cout << "Removed empty directory: " << path << std::endl;
            }
            catch (const std::exception& e)
            {
                std::cout << "Error: Failed to remove empty directory - " << e.what() << std::endl;
            }
        }
        else
        {
            std::cout << "Error: Directory is not empty. Use the -r flag to force remove." << std::endl;
        }
    }
    else if (fs::is_regular_file(path))
    {
        try
        {
            fs::remove(path);
            std::cout << "Removed file: " << path << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "Error: Failed to remove file - " << e.what() << std::endl;
        }
    }
    else
    {
        std::cout << "Error: Invalid path (not a file or directory)." << std::endl;
    }
}
