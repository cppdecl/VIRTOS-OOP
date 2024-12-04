#include "Shell.h"
#include "Core/System.h"
#include "Core/UserAccountControl/UserAccountControl.h"
#include "Core/ProgramManager/ProgramManager.h"
#include "Utils/Hash.h"
#include "Utils/Text.hh"

void Shell::Run() 
{
	System& sys = System::GetInstance();
	UserAccount* user = sys.GetUserAccountControl()->GetCurrentUser();
	if (!user)
		return;

    std::string line;

	std::string currentPath = user->GetCurrentPath();
	Console::Print("{}[{}] {} ~", Console::Color::GREEN, user->GetUsername(), currentPath);

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
	else if (command == "user")
	{
		if (args.size() == 0)
		{
			Console::Log("Usage: user <create/delete/root/unroot/list> <username> <password>");
			return;
		}

		std::string subcommand = args[0];
		args.erase(args.begin());

		if (subcommand == "create")
		{
			if (!user->IsRoot())
			{
				Console::Log("{}Permission Denied: You must be root to create a user.", Console::Color::RED);
				return;
			}

			if (args.size() < 2)
			{
				Console::Log("{}Usage: user create <username> <password>", Console::Color::RED);
				return;
			}

			Text::ToLower(args[0]);

			if (sys.GetUserAccountControl()->GetUser(args[0]))
			{
				Console::Log("User '{}' already exists.", args[0]);
				return;
			}

			UserAccount* newUser = sys.GetUserAccountControl()->CreateUser(new UserAccount());
			newUser->SetUsername(args[0]);
			newUser->SetPassword(args[1]);
			newUser->SetHomePath(Constants::UserBaseDir + args[0] + "/");

			IO::CreateDirectoryRecursive(newUser->GetHomePath());
			sys.GetUserAccountControl()->SaveUsers();
			Console::Log("User '{}' created.", args[0]);
		}
		else if (subcommand == "delete")
		{
			if (!user->IsRoot())
			{
				Console::Log("{}Permission Denied: You must be root to delete a user.", Console::Color::RED);
				return;
			}

			if (args.size() < 1)
			{
				Console::Log("Usage: user delete <username>");
				return;
			}

			Text::ToLower(args[0]);

			if (args[0] == "root")
			{
				Console::Log("Cannot delete 'root' user.");
				return;
			}

			if (!sys.GetUserAccountControl()->DeleteUser(args[0]))
			{
				Console::Log("User '{}' does not exist.", args[0]);
				return;
			}

			Console::Log("User '{}' deleted.", args[0]);
		}
		else if (subcommand == "root")
		{
			if (!user->IsRoot())
			{
				Console::Log("{}Permission Denied: You must be root to root a user.", Console::Color::RED);
				return;
			}

			if (args.size() < 1)
			{
				Console::Log("Usage: user root <username>");
				return;
			}

			UserAccount* targetUser = sys.GetUserAccountControl()->GetUser(args[0]);
			if (!targetUser)
			{
				Console::Log("User '{}' does not exist.", args[0]);
				return;
			}

			if (targetUser->IsRoot())
			{
				Console::Log("User '{}' is already root.", args[0]);
				return;
			}

			targetUser->SetRoot(true);

			sys.GetUserAccountControl()->SaveUsers();

			Console::Log("User '{}' is now root.", args[0]);
			
		}
		else if (subcommand == "unroot")
		{
			if (!user->IsRoot())
			{
				Console::Log("{}Permission Denied: You must be root to unroot a user.", Console::Color::RED);
				return;
			}

			if (args.size() < 1)
			{
				Console::Log("Usage: user unroot <username>");
				return;
			}

			UserAccount* targetUser = sys.GetUserAccountControl()->GetUser(args[0]);
			if (!targetUser)
			{
				Console::Log("User '{}' does not exist.", args[0]);
				return;
			}

			if (!targetUser->IsRoot())
			{
				Console::Log("User '{}' is not root.", args[0]);
				return;
			}

			if (targetUser->GetUsername() == "root")
			{
				Console::Log("Cannot unroot 'root' user.");
				return;
			}

			targetUser->SetRoot(false);

			sys.GetUserAccountControl()->SaveUsers();

			Console::Log("User '{}' is no longer root.", args[0]);
		}
		else if (subcommand == "list")
		{
			Console::Log("List of users in this system:");
			sys.GetUserAccountControl()->Users([](int index, UserAccount* user) {
				Console::Log("{}- {}{}{}{}", Console::Color::GREEN, Console::Color::WHITE, user->GetUsername(), Console::Color::RED, user->IsRoot() ? " (root)" : "");
			});
		}
		else
		{
			Console::Log("Unknown subcommand '{}'", subcommand);
		}
	}
	else if (sys.GetProgramManager()->Run(command, args))
	{
		
	}
	else
	{
		Console::Log("{}Unknown command. '{}'. Not a program or a cmdlet.", Console::Color::RED, command);
	}
}