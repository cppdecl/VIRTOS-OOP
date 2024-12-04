#include "ListFilesProgram.h"

#include "Constants/Paths.hh"
#include "Core/System.h"
#include "Core/UserAccountControl/UserAccountControl.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

ListFilesProgram::ListFilesProgram()
{

}

ListFilesProgram::~ListFilesProgram()
{
}

void ListFilesProgram::Run(std::vector<std::string> args)
{
    System& sys = System::GetInstance();
    UserAccount* user = sys.GetUserAccountControl()->GetCurrentUser();
    std::string path = user->GetCurrentPath() + (args.size() == 0 ? "" : args[0]);

    int fileCount = 0;
    std::string files = "";
    
    for (const auto& entry : fs::directory_iterator(path))
    {
        fileCount++;
        files += entry.path().filename().string() + std::string(" ");
    }

    if (fileCount == 0)
    {
        std::cout << "No files found in " << path << std::endl;
        return;
    }

    std::string out = "Found " + std::to_string(fileCount) + " files in " + path + ": \n" + files;

    std::cout << out << std::endl;
}