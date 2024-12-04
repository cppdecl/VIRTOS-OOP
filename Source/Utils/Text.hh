#ifndef UTILS_TEXT_HH
#define UTILS_TEXT_HH


#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>
#include <sstream>
#include <string>
#include <unordered_set>
#include <numeric>
#include <string>
#include <format>

namespace Text
{
	template <typename... Args>
	inline std::string Format(const std::string& format, Args&&... args)
	{
        return std::vformat(format, std::make_format_args(args...));
	}

	inline int FastAtoi( const char * str )
	{
		int val = 0;
		while( *str ) {
			val = val*10 + (*str++ - '0');
		}
		return val;
	}

	inline std::string FormatStringList(const std::vector<std::string> words) 
	{
		size_t n = words.size();
		if (n == 0) {
			return "";
		}
		if (n == 1) {
			return words[0];
		}
		if (n == 2) {
			return words[0] + " and " + words[1];
		}

		std::ostringstream oss;
		for (size_t i = 0; i < n - 1; ++i) {
			oss << words[i] << ", ";
		}
		oss << "and " << words[n - 1];
		return oss.str();
	}
	
	inline auto IsInteger(const std::string& str) -> bool
	{
		return !str.empty() && std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isdigit(c); });
	}

	inline auto ReplaceAll(std::string& mainString, const std::string& toReplace, const std::string& replaceWith) -> void
    {
        size_t pos = mainString.find(toReplace);
        size_t lengthOfReplaceWith = replaceWith.length();
        size_t lengthOfToReplace = toReplace.length();

        while (pos != std::string::npos) 
        {
            mainString.replace(pos, lengthOfToReplace, replaceWith);
            pos = mainString.find(toReplace, pos + lengthOfReplaceWith);
        }
    }

    
	
	inline std::string ItemNameToEnumFormat(std::string name)
	{
		std::string itemName = name;
		// Formatting itemName as per the specified rules
		std::transform(itemName.begin(), itemName.end(), itemName.begin(),
						[](unsigned char c) { return std::toupper(c); }); // Convert to uppercase

		itemName.erase(std::remove_if(itemName.begin(), itemName.end(), ::isspace), itemName.end()); // Remove spaces

		for (char &c : itemName) {
			if (!isalnum(c) && c != '_') {
				c = '_'; // Replace symbols with underscore
			}
		}

		return itemName;
	}

	inline int ToInt(const std::string& str) 
	{
		return std::atoi(str.c_str());
	}

	template <typename T>
	inline std::string ToString(const T &value)
	{
		return std::to_string(value);
	}

	inline void LimitStringSize(std::string &str, size_t size)
	{
		if (str.length() > size)
		{
			str.erase(size);
		}
	}

	inline std::pair<std::string, std::string> SplitString(const std::string& input, char delimiter) 
	{
		size_t delimiterPos = input.find(delimiter);

		if (delimiterPos == std::string::npos) {
			return std::make_pair(input, "");
		}

		std::string part1 = input.substr(0, delimiterPos);
		std::string part2 = input.substr(delimiterPos + 1);

		return std::make_pair(part1, part2);
	}

	inline bool ContainsCharOnly (const std::string& str, char character) 
	{
		const char* c_str = str.c_str();
		const char* end = c_str + str.length();

		while (c_str < end) {
			if (*c_str != character) {
				return false;
			}
			++c_str;
		}

		return true;
	}

	inline bool IsAlphanumeric(const std::string& str) 
	{
		for (char c : str) {
			if (!std::isalnum(c)) {
				return false;
			}
		}
		return true;
	}

	inline void RemoveSymbols(std::string &str) 
	{
		str.erase(std::remove_if(str.begin(), str.end(), [](unsigned char c) {
			return !std::isalnum(c);
		}), str.end());
	}


	inline void RemoveBadCharacters(std::string &str)
	{
		str.erase(std::remove_if(str.begin(), str.end(),
								 [](char c) {
									 return !std::isalnum(c) && c != ' ' && c != '!' && c != '"' && c != '#' &&
											c != '$' && c != '%' && c != '&' && c != '\'' && c != '(' && c != ')' &&
											c != '*' && c != '+' && c != ',' && c != '-' && c != '.' && c != '/' &&
											c != ':' && c != ';' && c != '<' && c != '=' && c != '>' && c != '?' &&
											c != '@' && c != '[' && c != '\\' && c != ']' && c != '^' && c != '_' &&
											c != '`' && c != '{' && c != '}' && c != '~';
								 }),
				  str.end());
		str.erase(std::remove(str.begin(), str.end(), '|'), str.end());
	}

	inline void TrimSpacesLeftOfString(std::string &s)
	{
		const char *start = s.c_str();
		const char *end	  = start + s.size();

		while (start < end && std::isspace(*start))
		{
			start++;
		}

		s.erase(0, start - s.c_str());
	}

	inline void TrimSpacesRightOfString(std::string &s)
	{
		const char *start		   = s.c_str();
		const char *end			   = start + s.size();
		const char *last_non_space = end;

		while (last_non_space > start && std::isspace(*(last_non_space - 1)))
		{
			last_non_space--;
		}

		s.erase(last_non_space - s.c_str(), end - last_non_space);
	}

	inline void TrimSpaceBothSidesOfString(std::string &s)
	{
		TrimSpacesLeftOfString(s);
		TrimSpacesRightOfString(s);
	}

	inline void RemoveExtraSpaces(std::string &str)
	{
		// Remove leading whitespace
		str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int c) { return !std::isspace(c); }));

		// Remove trailing whitespace
		str.erase(std::find_if(str.rbegin(), str.rend(), [](int c) { return !std::isspace(c); }).base(), str.end());

		// Remove extra whitespace between words
		auto last =
			std::unique(str.begin(), str.end(), [](char a, char b) { return std::isspace(a) && std::isspace(b); });
		str.erase(last, str.end());
	}

	inline auto RemoveSpaces(std::string &str) -> void
	{
		str.erase(remove(str.begin(), str.end(), ' '), str.end());
	}

	inline auto StartsWithChar(std::string& str, char what) -> bool 
	{
		if (str.find(what) == 0)
			return true;

		return false;
	}

	inline auto StartsWithChar(std::string& str, std::string what) -> bool 
	{
		if (str.find(str) == 0)
			return true;

		return false;
	}

	inline bool StartsWith(const std::string& str, const std::string& prefix) 
	{
		if (str.length() < prefix.length()) {
			return false; // The string is shorter than the prefix
		}

		return str.substr(0, prefix.length()) == prefix;
	}

	inline void RemoveFirstChar(std::string& input) 
	{
		if (!input.empty()) 
		{
        	input.erase(input.begin());
    	}	
	}

	inline auto SwitchCharCase(char &letter) -> void
	{
		letter ^= 32;
	}

	inline auto ToUpper(std::string &str) -> void
	{
		for (int i = 0; str[i] != '\0'; i++)
		{
			if (str[i] >= 'a' && str[i] <= 'z')	 
				str[i] = str[i] - 32;	
		}
	}

	inline auto ToLower(std::string &str) -> void
	{
		for (int i = 0; str[i] != '\0'; i++)
		{
			if (str[i] >= 'A' && str[i] <= 'Z')	 // checking for uppercase characters
				str[i] = str[i] + 32;			 // converting uppercase to lowercase
		}
	}

	inline auto RemoveGTColors(std::string &from) -> void
	{
		std::string str = "`";

		size_t start_pos = 0;
		bool   found	 = false;
		while (((start_pos = from.find(str)) != std::string::npos))
		{
			from.erase(start_pos, str.length() + 1);
		}
	}

	inline auto RemoveStringAfter(std::string &from, std::string &str, uint64_t len = 1) -> void
	{
		size_t start_pos = 0;
		bool   found	 = false;
		while (((start_pos = from.find(str)) != std::string::npos))
		{
			from.erase(start_pos, str.length() + len);
		}
	}

	inline auto GenerateRandomString(uint64_t len) -> std::string
	{
		auto randchar = []() -> char {
			const char charset[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};
		std::string str(len, 0);
		std::generate_n(str.begin(), len, randchar);
		return str;
	}

	inline std::vector<int> SplitStringToInt(const std::string& input, char delimiter) 
	{
		std::vector<int> result;
		std::stringstream ss(input);
		std::string token;

		while (std::getline(ss, token, delimiter)) {
			try {
				size_t pos;
				int value = std::stoi(token, &pos);
				if (pos != token.size()) {
					throw std::invalid_argument("Non-integer value found");
				}
				result.push_back(value);
			} catch (const std::invalid_argument&) {
				throw std::invalid_argument("Non-integer value found");
			} catch (const std::out_of_range&) {
				throw std::out_of_range("Integer out of range");
			}
		}

		return result;
	}

	template<typename T>
	inline std::vector<T> SplitStringToIntType(const std::string& input, char delimiter) 
	{
		std::vector<T> result;
		std::stringstream ss(input);
		std::string token;

		while (std::getline(ss, token, delimiter)) 
		{
			std::istringstream iss(token);
			T value;
			
			if constexpr (std::is_same_v<T, int>) 
			{
				try 
				{
					int intValue;
					iss >> intValue;
					if (!iss.eof() || !iss.fail()) 
					{
						throw std::invalid_argument("Non-integer value found");
					}
					result.push_back(intValue);
				} 
				catch (const std::invalid_argument&) 
				{
					throw std::invalid_argument("Non-integer value found");
				} 
				catch (const std::out_of_range&) 
				{
					throw std::out_of_range("Integer out of range");
				}
			} 
			else if constexpr (std::is_enum_v<T>) 
			{
				int enumValue;
				iss >> enumValue;
				if (!iss.eof() || !iss.fail()) {
					throw std::invalid_argument("Invalid enum value");
				}
				T enumVal = static_cast<T>(enumValue);
				result.push_back(enumVal);
			} 
			else 
			{
				static_assert(std::is_enum_v<T> || std::is_integral_v<T>, "Unsupported type");
			}
		}

		return result;
	}

	inline auto CombineArgsToSentence(std::vector<std::string>& args) -> std::string 
	{
		return std::accumulate(args.begin(), args.end(), std::string(), [](const std::string& a, const std::string& b) { return a.empty() ? b : a + " " + b; });
	}

	inline auto Split(std::string &str) -> std::vector<std::string>
	{
		struct tokens : std::ctype<char>
		{
			tokens() : std::ctype<char>(get_table())
			{
			}

			static std::ctype_base::mask const *get_table()
			{
				typedef std::ctype<char>   cctype;
				static const cctype::mask *const_rc = cctype::classic_table();

				static cctype::mask rc[cctype::table_size];
				std::memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));

				rc[' '] = std::ctype_base::space;
				return &rc[0];
			}
		};

		std::stringstream ss(str);
		ss.imbue(std::locale(std::locale(), new tokens()));
		std::istream_iterator<std::string> begin(ss);
		std::istream_iterator<std::string> end;
		std::vector<std::string>		   vstrings(begin, end);
		return std::move(vstrings);
	}

	inline std::vector<std::string> Split(const std::string& str, char delimiter) 
	{
		std::vector<std::string> result;
		std::stringstream ss(str);
		std::string token;

		while (std::getline(ss, token, delimiter)) {
			result.push_back(token);
		}

		return result;
	}

	inline auto SplitDialogName(std::string &str) -> std::vector<std::string>
	{
		struct tokens : std::ctype<char>
		{
			tokens() : std::ctype<char>(get_table())
			{
			}

			static std::ctype_base::mask const *get_table()
			{
				typedef std::ctype<char>   cctype;
				static const cctype::mask *const_rc = cctype::classic_table();

				static cctype::mask rc[cctype::table_size];
				std::memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));

				rc['&'] = std::ctype_base::space;
				return &rc[0];
			}
		};

		std::stringstream ss(str);
		ss.imbue(std::locale(std::locale(), new tokens()));
		std::istream_iterator<std::string> begin(ss);
		std::istream_iterator<std::string> end;
		std::vector<std::string>		   vstrings(begin, end);
		return std::move(vstrings);
	}

	inline void ParseCommandString(const std::string &commandString, std::string &commandName,
								   std::vector<std::string> &commandArgs)
	{
		std::string cmdStr = commandString;

		// Remove the leading slash character from the command string, if it exists
		if (!commandString.empty() && commandString[0] == '/')
		{
			cmdStr = commandString.substr(1);
		}

		// Find the first space character to extract the command name
		std::size_t spacePos = cmdStr.find(' ');
		if (spacePos == std::string::npos)
		{
			// No arguments, the entire string is the command name
			commandName = cmdStr;
			return;
		}

		// Extract the command name
		commandName = cmdStr.substr(0, spacePos);

		Text::ToLower(commandName);

		// Extract the command arguments
		std::string		   argsString = cmdStr.substr(spacePos + 1);
		std::istringstream iss(argsString);
		std::string		   arg;
		while (iss >> arg)
		{
			if (arg.find('|') != std::string::npos)
			{
				// Remove the '|' character from the argument
				arg.erase(std::remove(arg.begin(), arg.end(), '|'), arg.end());
			}
			commandArgs.push_back(arg);
		}
	}

	inline auto RemoveSingleBacktick(std::string& str) -> void
	{
		size_t pos = 0;
		while ((pos = str.find("`", pos)) != std::string::npos) {
			if (pos == 0) {
				str.erase(pos, 1);
			} else {
				pos++; 
			}
		}
	}

	inline auto RemoveSubstring(std::string& str, const std::string& substr) -> void
	{
		size_t pos = str.find(substr);
		if (pos != std::string::npos) 
		{
			str.erase(pos, substr.length());
		}
	}

	inline std::string FormatBalance(uint64_t balance) 
	{
		std::string str = std::to_string(balance);
		std::string out = "";

		int count = 0;
		for (int i = str.size() - 1; i >= 0; i--) {
			out += str.at(i);
			count++;
			if (count % 3 == 0 && i != 0) {
				out += ",";
			}
		}
		
		std::reverse(out.begin(), out.end());
		return out;
	}

	inline auto HasText(const std::string& mainString, const std::string& substring, bool caseInsensitive = false) -> bool 
    {
        if (!caseInsensitive)
        {
            size_t pos = mainString.find(substring);
            return pos != std::string::npos;
        }

        std::string main = mainString;
        RemoveBadCharacters(main);
        ToLower(main);
        RemoveExtraSpaces(main);

        std::string sub = substring;
        RemoveBadCharacters(sub);
        ToLower(sub);
        RemoveExtraSpaces(sub);

        size_t pos = main.find(sub);
        return pos != std::string::npos;
    }

}  // namespace Text

#endif	// __MISC__TEXT__HH__
