#include "NanoProgram.h"
#include "Core/System.h"
#include "Core/UserAccountControl/UserAccountControl.h"
#include <iostream>
#include "Utils/Text.hh"
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <chrono>
#include <unordered_map>

NanoProgram::NanoProgram() {}

NanoProgram::~NanoProgram() {}

void NanoProgram::Run(std::vector<std::string> args)
{
    if (args.size() == 0)
    {
        std::cout << "Usage: nano <filename> <optional: text>" << std::endl;
        return;
    }

    System& sys = System::GetInstance();
    UserAccount* user = sys.GetUserAccountControl()->GetCurrentUser();

    std::string fileName = user->GetCurrentPath() + args[0];

    if (args.size() > 1) 
    {
        if (!user->IsRoot())
        {
            if (user->GetCurrentPath().find(user->GetHomePath()) == std::string::npos)
            {
                std::cout << "Permission denied: You can only create files in your home directory." << std::endl;
                return;
            }
        }

        if (std::filesystem::exists(fileName))
        {
            std::cout << "Error: File already exists." << std::endl;
            return;
        }
        
        args.erase(args.begin());
        std::string text = Text::CombineArgsToSentence(args);
        std::ofstream file(fileName);
        file << text;
        file.close();

        std::cout << "File written: " << fileName << std::endl;
    }
    else 
    {
        std::ifstream file(fileName);
        if (file.is_open())
        {
            std::string contents = "";
            std::getline(file, contents, '\0');
            std::cout << "File contents: " << contents << std::endl;
        }   
    }
}