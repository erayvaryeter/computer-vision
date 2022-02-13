#include <logger/logger.h>
#include <string/string.h>
#include <assertion/assertion.h>
#include <stdio.h>
#include <time.h>
#include "datetime/datetime.h"

namespace base {

std::shared_ptr<base::Logger> DateTime::m_logger = std::make_shared<base::Logger>();

DateTime::DateTime(std::chrono::year year, std::chrono::month month, std::chrono::day day, std::chrono::hours hours,
	std::chrono::minutes minutes, std::chrono::seconds seconds, std::chrono::milliseconds milliseconds) {

	m_milliseconds = milliseconds;
	m_seconds = seconds;
	m_minutes = minutes;
	m_hours = hours;
	m_day = day;
	m_month = month;
	m_year = year;

	std::stringstream ss;

	ss << m_milliseconds;
	ss >> m_millisecondsNumberStr;
	ss.clear();

	ss << m_seconds;
	ss >> m_secondsNumberStr;
	ss.clear();

	ss << m_minutes;
	ss >> m_minutesNumberStr;
	ss.clear();

	ss << m_hours;
	ss >> m_hoursNumberStr;
	ss.clear();

	ss << m_day;
	ss >> m_dayNumberStr;
	ss.clear();

	ss << m_month;
	ss >> m_monthNumberStr;
	ss.clear();

	ss << m_year;
	ss >> m_yearNumberStr;
	ss.clear();

	m_millisecondsNumber = base::String::ConvertStringToNumerical<int>(m_millisecondsNumberStr.c_str());
	m_secondsNumber = base::String::ConvertStringToNumerical<int>(m_secondsNumberStr.c_str());
	m_minutesNumber = base::String::ConvertStringToNumerical<int>(m_minutesNumberStr.c_str());
	m_hoursNumber = base::String::ConvertStringToNumerical<int>(m_hoursNumberStr.c_str());
	m_dayNumber = base::String::ConvertStringToNumerical<int>(m_dayNumberStr.c_str());
	m_monthNumber = base::String::ConvertStringToNumerical<int>(m_monthNumberStr.c_str());
	m_yearNumber = base::String::ConvertStringToNumerical<int>(m_yearNumberStr.c_str());

	CalculateTotalMs();
}

DateTime::DateTime(int year, int month, int day, int hours, int minutes, int seconds, int milliseconds) {
	m_millisecondsNumber = milliseconds;
	m_secondsNumber = seconds;
	m_minutesNumber = minutes;
	m_hoursNumber = hours;
	m_dayNumber = day;
	m_monthNumber = month;
	m_yearNumber = year;
	m_millisecondsNumberStr = base::String::ConvertNumericalToString<int>(m_millisecondsNumber);
	m_secondsNumberStr = base::String::ConvertNumericalToString<int>(m_secondsNumber);
	m_minutesNumberStr = base::String::ConvertNumericalToString<int>(m_minutesNumber);
	m_hoursNumberStr = base::String::ConvertNumericalToString<int>(m_hoursNumber);
	m_dayNumberStr = base::String::ConvertNumericalToString<int>(m_dayNumber);
	m_monthNumberStr = base::String::ConvertNumericalToString<int>(m_monthNumber);
	m_yearNumberStr = base::String::ConvertNumericalToString<int>(m_yearNumber);
	m_milliseconds = std::chrono::milliseconds(m_millisecondsNumber);
	m_seconds = std::chrono::seconds(m_secondsNumber);
	m_minutes = std::chrono::minutes(m_minutesNumber);
	m_hours = std::chrono::hours(m_hoursNumber);
	m_day = std::chrono::day(m_dayNumber);
	m_month = std::chrono::month(m_monthNumber);
	m_year = std::chrono::year(m_yearNumber);

	CalculateTotalMs();
}

DateTime::DateTime(std::string year, std::string month, std::string day, std::string hours, std::string minutes,
	std::string seconds, std::string milliseconds) {

	m_millisecondsNumberStr = milliseconds;
	m_secondsNumberStr = seconds;
	m_minutesNumberStr = minutes;
	m_hoursNumberStr = hours;
	m_dayNumberStr = day;
	m_monthNumberStr = month;
	m_yearNumberStr = year;

	m_millisecondsNumber = base::String::ConvertStringToNumerical<int>(m_millisecondsNumberStr.c_str());
	m_secondsNumber = base::String::ConvertStringToNumerical<int>(m_secondsNumberStr.c_str());
	m_minutesNumber = base::String::ConvertStringToNumerical<int>(m_minutesNumberStr.c_str());
	m_hoursNumber = base::String::ConvertStringToNumerical<int>(m_hoursNumberStr.c_str());
	m_dayNumber = base::String::ConvertStringToNumerical<int>(m_dayNumberStr.c_str());
	m_monthNumber = base::String::ConvertStringToNumerical<int>(m_monthNumberStr.c_str());
	m_yearNumber = base::String::ConvertStringToNumerical<int>(m_yearNumberStr.c_str());

	m_milliseconds = std::chrono::milliseconds(m_millisecondsNumber);
	m_seconds = std::chrono::seconds(m_secondsNumber);
	m_minutes = std::chrono::minutes(m_minutesNumber);
	m_hours = std::chrono::hours(m_hoursNumber);
	m_day = std::chrono::day(m_dayNumber);
	m_month = std::chrono::month(m_monthNumber);
	m_year = std::chrono::year(m_yearNumber);

	CalculateTotalMs();
}

DateTime::DateTime(std::string completeDateTime) {
	auto list1 = base::String::SplitString(completeDateTime, "-");
	ASSERT((list1.size() == 3), "Date time must have exactly 3 '-' delimiters", base::Logger::Severity::Error);
	m_yearNumberStr = list1[0];
	m_monthNumberStr = list1[1];
	auto list2 = base::String::SplitString(list1[2], " ");
	ASSERT((list2.size() == 2), "Date time must have space delimiter", base::Logger::Severity::Error);
	m_dayNumberStr = list2[0];
	auto list3 = base::String::SplitString(list2[1], ":");
	ASSERT((list3.size() == 3), "Date time must have : delimiter", base::Logger::Severity::Error);
	m_hoursNumberStr = list3[0];
	m_minutesNumberStr = list3[1];
	auto list4 = base::String::SplitString(list3[2], ".");
	ASSERT((list4.size() == 2), "Date time must have . delimiter for milliseconds", base::Logger::Severity::Error);
	m_secondsNumberStr = list4[0];
	m_millisecondsNumberStr = list4[1];

	m_millisecondsNumber = base::String::ConvertStringToNumerical<int>(m_millisecondsNumberStr.c_str());
	m_secondsNumber = base::String::ConvertStringToNumerical<int>(m_secondsNumberStr.c_str());
	m_minutesNumber = base::String::ConvertStringToNumerical<int>(m_minutesNumberStr.c_str());
	m_hoursNumber = base::String::ConvertStringToNumerical<int>(m_hoursNumberStr.c_str());
	m_dayNumber = base::String::ConvertStringToNumerical<int>(m_dayNumberStr.c_str());
	m_monthNumber = base::String::ConvertStringToNumerical<int>(m_monthNumberStr.c_str());
	m_yearNumber = base::String::ConvertStringToNumerical<int>(m_yearNumberStr.c_str());

	m_milliseconds = std::chrono::milliseconds(m_millisecondsNumber);
	m_seconds = std::chrono::seconds(m_secondsNumber);
	m_minutes = std::chrono::minutes(m_minutesNumber);
	m_hours = std::chrono::hours(m_hoursNumber);
	m_day = std::chrono::day(m_dayNumber);
	m_month = std::chrono::month(m_monthNumber);
	m_year = std::chrono::year(m_yearNumber);

	CalculateTotalMs();
}

void 
DateTime::CalculateTotalMs() {
	m_totalMs = 0.0;
	m_totalMs += YEAR * static_cast<double>(m_yearNumber);
	m_totalMs += MONTH * static_cast<double>(m_monthNumber);
	m_totalMs += DAY * static_cast<double>(m_dayNumber);
	m_totalMs += HOUR * static_cast<double>(m_hoursNumber);
	m_totalMs += MINUTE * static_cast<double>(m_minutesNumber);
	m_totalMs += SECOND * static_cast<double>(m_secondsNumber);
	m_totalMs += static_cast<double>(m_millisecondsNumber);
}

DateTime 
DateTime::GetCurrentDateTime() {
	std::stringstream ss;
	ss << floor<std::chrono::milliseconds>(std::chrono::system_clock::now());
	std::string date, time;
	ss >> date >> time;

	auto dateList = base::String::SplitString(date, "-");
	ASSERT((dateList.size() == 3), "Splitted date string must have exactly 3 elements", base::Logger::Severity::Error);

	auto timeList = base::String::SplitString(time, ":");
	ASSERT((timeList.size() == 3), "Splitted time string must have exactly 3 elements", base::Logger::Severity::Error);

	auto msList = base::String::SplitString(timeList[2], ".");
	ASSERT((msList.size() == 2), "Splitted millisecond time string must have exactly 2 elements", base::Logger::Severity::Error);

	return DateTime(dateList[0], dateList[1], dateList[2], timeList[0], timeList[1], msList[0], msList[1]);
}

std::string 
DateTime::GetCurrentDateTimeAsString() {
	auto currentDate = base::DateTime::GetCurrentDateTime();
	return base::DateTime::GetDateTimeAsString(currentDate);
}

std::string 
DateTime::GetDateTimeAsString(DateTime& dateTime) {
	std::stringstream ss;
	std::string milliseconds, seconds, minutes, hours, day, month, year;
	std::string delimiter1 = "-";
	std::string delimiter2 = ":";
	std::string delimiter3 = ".";
	std::string space = " ";

	return base::String::CombineStrings(
		{
			dateTime.GetYearStr(),
			delimiter1,
			dateTime.GetMonthStr(),
			delimiter1,
			dateTime.GetDayStr(),
			space,
			dateTime.GetHoursStr(),
			delimiter2,
			dateTime.GetMinutesStr(),
			delimiter2,
			dateTime.GetSecondsStr(),
			delimiter3,
			dateTime.GetMillisecondsStr()
		}
	);
}

void
DateTime::Append(int year, int month, int day, int hours, int minutes, int seconds, int milliseconds) {
	m_yearNumber += year;
	m_monthNumber += month;
	m_dayNumber += day;
	m_hoursNumber += hours;
	m_minutesNumber += minutes;
	m_secondsNumber += seconds;
	m_millisecondsNumber += milliseconds;
	// Check Millisecond Boundaries
	if (m_millisecondsNumber > 1000) {
		m_millisecondsNumber -= 1000;
		m_secondsNumber++;
	}
	if (m_millisecondsNumber < 0) {
		m_secondsNumber--;
		m_millisecondsNumber += 1000;
	}
	// Check Second Boundaries
	if (m_secondsNumber > 60) {
		m_secondsNumber -= 60;
		m_minutesNumber++;
	}
	if (m_secondsNumber < 0) {
		m_minutesNumber--;
		m_secondsNumber += 60;
	}
	// Check Minute Boundaries
	if (m_minutesNumber > 60) {
		m_minutesNumber -= 60;
		m_hoursNumber++;
	}
	if (m_minutesNumber < 0) {
		m_hoursNumber--;
		m_minutesNumber += 60;
	}
	// Check Hour Boundaries
	if (m_hoursNumber > 24) {
		m_hoursNumber -= 24;
		m_dayNumber++;
	}
	if (m_hoursNumber < 0) {
		m_dayNumber--;
		m_hoursNumber += 24;
	}
	// Check Day Boundaries
	if (m_dayNumber > 30) {
		m_dayNumber -= 30;
		m_monthNumber++;
	}
	if (m_dayNumber < 0) {
		m_monthNumber--;
		m_dayNumber += 30;
	}
	// Check Month Boundaries
	if (m_monthNumber > 12) {
		m_monthNumber -= 12;
		m_yearNumber++;
	}
	if (m_monthNumber < 0) {
		m_yearNumber--;
		m_monthNumber += 12;
	}

	m_millisecondsNumberStr = base::String::ConvertNumericalToString<int>(m_millisecondsNumber);
	m_secondsNumberStr = base::String::ConvertNumericalToString<int>(m_secondsNumber);
	m_minutesNumberStr = base::String::ConvertNumericalToString<int>(m_minutesNumber);
	m_hoursNumberStr = base::String::ConvertNumericalToString<int>(m_hoursNumber);
	m_dayNumberStr = base::String::ConvertNumericalToString<int>(m_dayNumber);
	m_monthNumberStr = base::String::ConvertNumericalToString<int>(m_monthNumber);
	m_yearNumberStr = base::String::ConvertNumericalToString<int>(m_yearNumber);
	m_milliseconds = std::chrono::milliseconds(m_millisecondsNumber);
	m_seconds = std::chrono::seconds(m_secondsNumber);
	m_minutes = std::chrono::minutes(m_minutesNumber);
	m_hours = std::chrono::hours(m_hoursNumber);
	m_day = std::chrono::day(m_dayNumber);
	m_month = std::chrono::month(m_monthNumber);
	m_year = std::chrono::year(m_yearNumber);

	CalculateTotalMs();
}

std::ostream& 
operator<<(std::ostream& output, DateTime& dt) {
	if (dt.GetYear() != 0)
		output << dt.GetYearStr() << " years ";
	if (dt.GetMonth() != 0)
		output << dt.GetMonthStr() << " months ";
	if (dt.GetDay() != 0)
		output << dt.GetDayStr() << " days ";
	if (dt.GetHours() != 0)
		output << dt.GetHoursStr() << " hours ";
	if (dt.GetMinutes() != 0)
		output << dt.GetMinutesStr() << " mins ";
	if (dt.GetSeconds() != 0)
		output << dt.GetSecondsStr() << " secs ";
	if (dt.GetMilliseconds() != 0)
		output << dt.GetMillisecondsStr() << " msecs ";
	return output;
}

DateTime 
operator+(DateTime& dt1, DateTime dt2) {
	DateTime result(dt1.GetYear(), dt1.GetMonth(), dt1.GetDay(), dt1.GetHours(), dt1.GetMinutes(), dt1.GetSeconds(), dt1.GetMilliseconds());
	result.Append(dt2.GetYear(), dt2.GetMonth(), dt2.GetDay(), dt2.GetHours(), dt2.GetMinutes(), dt2.GetSeconds(), dt2.GetMilliseconds());
	return result;
}

DateTime 
operator-(DateTime& dt1, DateTime dt2) {
	DateTime result(dt1.GetYear(), dt1.GetMonth(), dt1.GetDay(), dt1.GetHours(), dt1.GetMinutes(), dt1.GetSeconds(), dt1.GetMilliseconds());
	result.Append(-1 * dt2.GetYear(), -1 * dt2.GetMonth(), -1 * dt2.GetDay(), -1 * dt2.GetHours(),
		-1 * dt2.GetMinutes(), -1 * dt2.GetSeconds(), -1 * dt2.GetMilliseconds());
	return result;
}

}