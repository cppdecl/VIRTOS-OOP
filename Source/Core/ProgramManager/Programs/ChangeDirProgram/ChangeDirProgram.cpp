#include "ChangeDirProgram.h"

#include "Constants/Paths.hh"
#include "Core/System.h"
#include "Core/UserAccountControl/UserAccountControl.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

ChangeDirProgram::ChangeDirProgram()
{

}

ChangeDirProgram::~ChangeDirProgram()
{
}

void ChangeDirProgram::Run(std::vector<std::string> args)
{
    System& sys = System::GetInstance();
    UserAccount* user = sys.GetUserAccountControl()->GetCurrentUser();
    
    if (args.size() == 0)
    {
        std::cout << "Usage: changedir <path>" << std::endl;
        return;
    }

    std::string path = args[0];

    if (path == "..")
    {
        std::string currentPath = user->GetCurrentPath();
        if (currentPath == Constants::OSDir)
        {
            std::cout << "You're already at the base directory." << std::endl;
            return;
        }

        if (!currentPath.empty() && currentPath.back() == '/')
        {
            currentPath.pop_back();
        }

        size_t lastSlash = currentPath.find_last_of('/');
        if (lastSlash != std::string::npos)
        {
            currentPath = currentPath.substr(0, lastSlash + 1); 
        }

        user->SetCurrentPath(currentPath);
        return;
    }

    if (path == ".")
    {
        return;
    }

    if (path == "/")
    {
        if (user->IsRoot())
        {
            std::cout << "Changing to root directory." << std::endl;
            user->SetCurrentPath(Constants::OSDir);
            return;
        }

        std::cout << "Permission denied." << std::endl;
        return;
    }

    if (path == "~/")
    {
        std::cout << "Changing to home directory." << std::endl;
        user->SetCurrentPath(user->GetHomePath());
        return;
    }

    std::string newPath = user->GetCurrentPath() + path;

    if (!fs::exists(newPath))
    {
        std::cout << "Path does not exist." << std::endl;
        return;
    }

    if (!fs::is_directory(newPath))
    {
        std::cout << "Path is not a directory." << std::endl;
        return;
    }

    if (!user->IsRoot() && newPath.find(Constants::UserBaseDir) == 0)
    {
        std::string targetUserHome = newPath.substr(Constants::UserBaseDir.size());
        size_t nextSlash = targetUserHome.find('/');
        if (nextSlash != std::string::npos)
        {
            targetUserHome = targetUserHome.substr(0, nextSlash);

            if (targetUserHome != user->GetUsername())
            {
                std::cout << "Permission denied: Cannot access another user's home directory." << std::endl;
                return;
            }
        }
    }

    if (newPath.back() != '/')
    {
        newPath += "/";
    }

    user->SetCurrentPath(newPath);
}