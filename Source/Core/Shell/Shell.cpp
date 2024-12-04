#include "Shell.h"
#include "Core/System.h"
#include "Core/UserAccountControl/UserAccountControl.h"
#include "Utils/Hash.h"

void Shell::Run() 
{
	System& sys = System::GetInstance();
	UserAccount* user = sys.GetUserAccountControl()->GetCurrentUser();
	if (!user)
		return;

    std::string line;

	Console::Print("{}[{}] ~", Console::Color::GREEN, user->GetUsername());

	if (!std::getline(std::cin, line))
	{
		return;
	}

	if (line.empty())
	{
		return;
	}

	std::istringstream		 linestream(line);
	std::string				 command;
	std::vector<std::string> args;

	if (std::getline(linestream, command, ' '))
	{
		std::string param;
		while (std::getline(linestream, param, ' '))
		{
			args.push_back(param);
		}
	}

	Console::OverwritePreviousLine();

    if (command == "q" || command == "quit" || command == "exit" || command == "shutdown" || command == "s")
	{
		sys.Shutdown();
	}
	else if (command == "logout")
	{
		sys.GetUserAccountControl()->Logout();
	}
	/*else if (sys.GetProgramManager()->RunProgram(command, args))
	{
		// Program ran successfully
	}*/
	else
	{
		Console::Log("{}Unknown command. '{}'. Not a program or a cmdlet.", Console::Color::RED, command);
	}
}