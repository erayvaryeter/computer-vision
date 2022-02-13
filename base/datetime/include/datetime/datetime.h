#pragma once

#include <memory>
#include <chrono>

using namespace std::chrono_literals;

namespace base {

class Logger;

class DateTime {

public:
	DateTime(std::chrono::year year, std::chrono::month month, std::chrono::day day, std::chrono::hours hours, 
			 std::chrono::minutes minutes, std::chrono::seconds seconds, std::chrono::milliseconds milliseconds);
	DateTime(int year, int month, int day, int hours, int minutes, int seconds, int milliseconds);
	DateTime(std::string year, std::string month, std::string day, std::string hours, std::string minutes,
			 std::string seconds, std::string milliseconds);
	DateTime(std::string completeDateTime);

	static DateTime GetCurrentDateTime();
	static std::string GetCurrentDateTimeAsString();
	static std::string GetDateTimeAsString(DateTime& dateTime);

	void Append(int year, int month, int day, int hours, int minutes, int seconds, int milliseconds);

	friend std::ostream& operator<<(std::ostream& output, DateTime& D);
	friend DateTime operator+(DateTime& dt1, DateTime dt2);
	friend DateTime operator-(DateTime& dt1, DateTime dt2);

	double ToMsec() { return m_totalMs; }
	double ToSec() { return m_totalMs / SECOND; }
	double ToMin() { return m_totalMs / MINUTE; }
	double ToHour() { return m_totalMs / HOUR; }
	double ToDay() { return m_totalMs / DAY; }
	double ToMonth() { return m_totalMs / MONTH; }
	double ToYear() { return m_totalMs / YEAR; }

	// GETTERS
	std::chrono::milliseconds GetChronoMilliseconds() { return m_milliseconds; }
	std::chrono::seconds GetChronoSeconds() { return m_seconds; }
	std::chrono::minutes GetChronoMinutes() { return m_minutes; }
	std::chrono::hours GetChronoHours() { return m_hours; }
	std::chrono::day GetChronoDay() { return m_day; }
	std::chrono::month GetChronoMonth() { return m_month; }
	std::chrono::year GetChronoYear() { return m_year; }

	int GetMilliseconds() { return m_millisecondsNumber; }
	int GetSeconds() { return m_secondsNumber; }
	int GetMinutes() { return m_minutesNumber; }
	int GetHours() { return m_hoursNumber; }
	int GetDay() { return m_dayNumber; }
	int GetMonth() { return m_monthNumber; }
	int GetYear() { return m_yearNumber; }

	std::string GetMillisecondsStr() { return m_millisecondsNumberStr; }
	std::string GetSecondsStr() { return m_secondsNumberStr; }
	std::string GetMinutesStr() { return m_minutesNumberStr; }
	std::string GetHoursStr() { return m_hoursNumberStr; }
	std::string GetDayStr() { return m_dayNumberStr; }
	std::string GetMonthStr() { return m_monthNumberStr; }
	std::string GetYearStr() { return m_yearNumberStr; }

private:
	void CalculateTotalMs();

	std::chrono::milliseconds m_milliseconds;
	std::chrono::seconds m_seconds;
	std::chrono::minutes m_minutes;
	std::chrono::hours m_hours;
	std::chrono::day m_day;
	std::chrono::month m_month;
	std::chrono::year m_year;

	int m_millisecondsNumber = 0;
	int m_secondsNumber = 0;
	int m_minutesNumber = 0;
	int m_hoursNumber = 0;
	int m_dayNumber = 0;
	int m_monthNumber = 0;
	int m_yearNumber = 0;

	std::string m_millisecondsNumberStr = "";
	std::string m_secondsNumberStr = "";
	std::string m_minutesNumberStr = "";
	std::string m_hoursNumberStr = "";
	std::string m_dayNumberStr = "";
	std::string m_monthNumberStr = "";
	std::string m_yearNumberStr = "";

	static std::shared_ptr<base::Logger> m_logger;

	double m_totalMs = 0.0;

	static constexpr double SECOND = 1000.0;
	static constexpr double MINUTE = 60.0 * SECOND;
	static constexpr double HOUR = 60.0 * MINUTE;
	static constexpr double DAY = 24.0 * HOUR;
	static constexpr double MONTH = 30.0 * DAY;
	static constexpr double YEAR = 12.0 * MONTH;
};

} // namespace base