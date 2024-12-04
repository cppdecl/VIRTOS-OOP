#ifndef UTILS_CLOCK_HH
#define UTILS_CLOCK_HH

#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <string>
#include <cstdint>
#include <format>

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
using Duration	= std::chrono::high_resolution_clock::duration;

namespace Clock
{
	inline auto Now() -> TimePoint
	{
		return std::chrono::high_resolution_clock::now();
	}

	inline auto ElapsedTime(const TimePoint& start, const TimePoint& end) -> Duration
	{
		return end - start;
	}

	inline auto GetSecondsSinceEpoch() -> int32_t
	{
		return std::chrono::duration_cast<std::chrono::seconds>(Now().time_since_epoch()).count();
	}

	inline auto TimePointToEpoch(TimePoint timePoint) -> int32_t
	{
		return std::chrono::duration_cast<std::chrono::seconds>(timePoint.time_since_epoch()).count();
	}

	inline auto EpochToTimePoint(int32_t secondsSinceEpoch) -> TimePoint
	{
		TimePoint dt{std::chrono::seconds(secondsSinceEpoch)};
		return dt;
	}

	inline auto TimePointToEpochMilli(TimePoint timePoint) -> uint64_t
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(timePoint.time_since_epoch()).count();
	}

	inline auto EpochMilliToTimePoint(uint64_t millisecondsSinceEpoch) -> TimePoint
	{
		TimePoint dt{std::chrono::milliseconds(millisecondsSinceEpoch)};
		return dt;
	}

	inline auto DiffNano(const TimePoint& start) -> long long
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(ElapsedTime(start, Now())).count();
	}

	inline auto DiffMicro(const TimePoint& start) -> long long
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(ElapsedTime(start, Now())).count();
	}

	inline auto DiffMilli(const TimePoint& start) -> long long
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime(start, Now())).count();
	}

	inline auto DiffSec(const TimePoint& start) -> double
	{
		return std::chrono::duration_cast<std::chrono::duration<double>>(ElapsedTime(start, Now())).count();
	}

	inline auto DiffMin(const TimePoint& start) -> double
	{
		return DiffSec(start) / 60.0;
	}

	inline auto DiffHour(const TimePoint& start) -> double
	{
		return DiffSec(start) / 3600.0;
	}

	inline auto DiffDay(const TimePoint& start) -> double
	{
		return DiffSec(start) / 86400.0;
	}

	inline auto DiffWeek(const TimePoint& start) -> double
	{
		return DiffSec(start) / 604800.0;
	}

	inline auto DiffMonth(const TimePoint& start) -> double
	{
		return DiffSec(start) / 2592000.0;
	}

	inline auto DiffYear(const TimePoint& start) -> double
	{
		return DiffSec(start) / 31536000.0;
	}

	inline auto FormatTimer(uint32_t ms) -> std::string
	{
		unsigned int seconds = ms / 1000;
		unsigned int minutes = seconds / 60;
		seconds %= 60;
		ms %= 1000;

		std::ostringstream oss;
		oss << minutes << ":" << std::setw(2) << std::setfill('0') << seconds << "." << std::setw(2) << std::setfill('0') << ms / 10;
		return oss.str();
	}

	inline auto FormatElapsed(TimePoint lastSomething, bool extended = true) -> std::string 
	{
		if (Clock::DiffSec(lastSomething) < 60)
			return std::format("{}", extended ? "a few secs ago" : std::format("{}s", (int)Clock::DiffSec(lastSomething)));
		else if (Clock::DiffMin(lastSomething) < 60)
			return std::format("{}", extended ? "a few mins ago" : std::format("{}m", (int)Clock::DiffMin(lastSomething)));
		else if (Clock::DiffHour(lastSomething) == 1)
			return std::format("{}", extended ? "an hour ago" : std::format("{}h", (int)Clock::DiffHour(lastSomething)));
		else if (Clock::DiffHour(lastSomething) > 1 && Clock::DiffHour(lastSomething) < 24)
			return std::format("{}", extended ? std::format("{} hours ago", (int)Clock::DiffHour(lastSomething)) : std::format("{}h", (int)Clock::DiffHour(lastSomething)));
		else if (Clock::DiffDay(lastSomething) == 1)
			return std::format("{}", extended ? "a day ago" : std::format("{}d", Clock::DiffDay(lastSomething)));
		else 
			return std::format("{}", extended ? std::format("{} days ago", (int)Clock::DiffDay(lastSomething)) : std::format("{}d", (int)Clock::DiffDay(lastSomething)));
	}

	inline auto SecondsToString(const std::chrono::seconds& time, bool yearAndDayOnly = false) -> std::string
    {
		static int year_count = 365 * 24 * 3600;
        static int day_count = 24 * 3600;
        static int hour_count = 3600;
        static int min_count = 60;

        int n = time.count();
		
		int year = n / year_count;
		n %= year_count;
        int day  = n / day_count;
        n %= day_count;
        int hour = n / hour_count;
        n %= hour_count;
        int min  = n / min_count;
        n %= min_count;
        int sec  = n;

        std::string result;

		if (yearAndDayOnly)
		{
			if (year != 0)
				result.append(std::format("{} years, ", year));
			if (day != 0)
				result.append(std::format("{} days", day));

			return result;
		}

		if (year != 0)
			result.append(std::format("{} years, ", year));
        if (day != 0)
            result.append(std::format("{} days, ", day));
        if (hour != 0)
            result.append(std::format("{} hours, ", hour));
        if (min != 0)
            result.append(std::format("{} mins, ", min));

        result.append(std::format("{} secs", sec));

        return result;
    }

	inline auto TimePointToTimeStamp (const std::chrono::system_clock::time_point& timePoint) -> std::string 
	{
		std::time_t time_tValue = std::chrono::system_clock::to_time_t(timePoint);
		
		std::tm* localTime = std::localtime(&time_tValue);

		std::stringstream ss;
		ss << std::put_time(localTime, "%B %d, %Y - %l:%M %p");

		return ss.str();
	}

	inline auto GetTimestamp() -> std::string 
	{
		std::time_t now = std::time(nullptr);
		std::tm* curtime = std::localtime(&now);

		const std::string daysOfWeek[] = {
			"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
		};
		const std::string monthsOfYear[] = {
			"January", "February", "March", "April", "May", "June", "July",
			"August", "September", "October", "November", "December"
		};

		std::stringstream formattedTime;
		formattedTime << std::put_time(curtime, "%A, %B %d, %Y - %I:%M %p");

		return std::format("{}", formattedTime.str());
	}

	inline auto GetTradeTimestamp() -> std::string
	{
		std::time_t currentTime = std::time(nullptr);
		std::tm* timeStruct = std::localtime(&currentTime);

		if (timeStruct == nullptr) {
			std::cerr << "Error getting current time\n";
			return "";
		}

		std::ostringstream output;
		output << std::put_time(timeStruct, "%I:%M %p on %m/%d");
		return output.str();
	}
};

#endif	// UTILS_CLOCK_HH