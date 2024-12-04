#ifndef UTILS_CONSOLE_HH
#define UTILS_CONSOLE_HH

#define _WINSOCKAPI_
#include <Windows.h>
#include <chrono>
#include <format>
#include <iostream>
#include <string>
#include <mutex>

namespace Console
{
	inline bool ShouldShowCursor = true;

	inline void ShowCursor() 
	{
		ShouldShowCursor = true;
	}

	inline void HideCursor() 
	{
		ShouldShowCursor = false;
	}

	namespace Color
	{
		// Reset
		const std::string RESET = "\033[0m";

		// Text Colors
		const std::string BLACK	  = "\033[30m";
		const std::string RED	  = "\033[31m";
		const std::string GREEN	  = "\033[32m";
		const std::string YELLOW  = "\033[33m";
		const std::string BLUE	  = "\033[34m";
		const std::string MAGENTA = "\033[35m";
		const std::string CYAN	  = "\033[36m";
		const std::string WHITE	  = "\033[37m";

		// Background Colors
		const std::string BG_BLACK	 = "\033[40m";
		const std::string BG_RED	 = "\033[41m";
		const std::string BG_GREEN	 = "\033[42m";
		const std::string BG_YELLOW	 = "\033[43m";
		const std::string BG_BLUE	 = "\033[44m";
		const std::string BG_MAGENTA = "\033[45m";
		const std::string BG_CYAN	 = "\033[46m";
		const std::string BG_WHITE	 = "\033[47m";

		// Text Formatting
		const std::string BOLD		= "\033[1m";
		const std::string UNDERLINE = "\033[4m";

		// Intensity
		const std::string INTENSE = "\033[9";  // Use with color codes

		// Dynamic Functions for High Intensity Colors
		inline std::string Intense(const std::string& colorCode)
		{
			return INTENSE + colorCode.substr(2, 2) + "m";
		}

		// High Intensity Text Colors
		const std::string INTENSE_BLACK	  = Intense(BLACK);
		const std::string INTENSE_RED	  = Intense(RED);
		const std::string INTENSE_GREEN	  = Intense(GREEN);
		const std::string INTENSE_YELLOW  = Intense(YELLOW);
		const std::string INTENSE_BLUE	  = Intense(BLUE);
		const std::string INTENSE_MAGENTA = Intense(MAGENTA);
		const std::string INTENSE_CYAN	  = Intense(CYAN);
		const std::string INTENSE_WHITE	  = Intense(WHITE);

		// High Intensity Background Colors
		inline std::string BGIntense(const std::string& colorCode)
		{
			return "\033[10" + colorCode.substr(2, 2) + "m";
		}

		const std::string BG_INTENSE_BLACK	 = BGIntense(BLACK);
		const std::string BG_INTENSE_RED	 = BGIntense(RED);
		const std::string BG_INTENSE_GREEN	 = BGIntense(GREEN);
		const std::string BG_INTENSE_YELLOW	 = BGIntense(YELLOW);
		const std::string BG_INTENSE_BLUE	 = BGIntense(BLUE);
		const std::string BG_INTENSE_MAGENTA = BGIntense(MAGENTA);
		const std::string BG_INTENSE_CYAN	 = BGIntense(CYAN);
		const std::string BG_INTENSE_WHITE	 = BGIntense(WHITE);
	}

	inline void Clear() 
	{
		system("cls");
	}

	inline void Init()
	{
		system("cls");

		HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD  consoleMode;
		GetConsoleMode(handleOut, &consoleMode);
		consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		consoleMode |= DISABLE_NEWLINE_AUTO_RETURN;
		SetConsoleMode(handleOut, consoleMode);
	}

	inline std::mutex  LogMutex;
	inline std::string LastLoggedMsg = "";
	inline std::string ArrowColor	 = Console::Color::GREEN;

	inline void OverwritePreviousLine()
	{
		std::cout << "\r\33[2K";
	}

	template <typename... Args>
	static void Log(const std::string& format, Args&&... args)
	{
		LogMutex.lock();

		if (LastLoggedMsg == ArrowColor + "> ")
		{
			Console::OverwritePreviousLine();
		}

		std::cout << std::vformat(format, std::make_format_args(args...)) << "\n";
		LastLoggedMsg = std::vformat(format, std::make_format_args(args...));

		if (LastLoggedMsg != ArrowColor + "> " && ShouldShowCursor)
		{
			std::cout << ArrowColor + "> ";
			LastLoggedMsg = ArrowColor + "> ";
		}

		std::cout << Console::Color::RESET;

		LogMutex.unlock();
	}

	template <typename... Args>
	static void PrintToCurrentLine(const std::string& format, Args&&... args)
	{
		LogMutex.lock();

		if (LastLoggedMsg == ArrowColor + "> ")
		{
			Console::OverwritePreviousLine();
		}

		std::cout << std::vformat(format, std::make_format_args(args...));
		LastLoggedMsg += std::vformat(format, std::make_format_args(args...));

		std::cout << Console::Color::RESET;

		LogMutex.unlock();
	
	}

	static void NextLine()
	{
		LogMutex.lock();
		std::cout << "\n";
		LastLoggedMsg = "";
		LogMutex.unlock();
	}

	template <typename... Args>
	static void Print(const std::string& format, Args&&... args)
	{
		LogMutex.lock();

		if (LastLoggedMsg == ArrowColor + "> ")
		{
			Console::OverwritePreviousLine();
		}

		std::cout << std::vformat(format, std::make_format_args(args...));
		LastLoggedMsg = std::vformat(format, std::make_format_args(args...));

		if (LastLoggedMsg != ArrowColor + "> " && ShouldShowCursor)
		{
			std::cout << ArrowColor + "> ";
			LastLoggedMsg = ArrowColor + "> ";
		}

		std::cout << Console::Color::RESET;

		LogMutex.unlock();
	}
}

#endif	// UTILS_CONSOLE_HH