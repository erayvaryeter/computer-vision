#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>
#include <datetime/datetime.h>
#include <logger/logger.h>
#include <iostream>
#include <thread>

auto datetimeLogger = std::make_shared<base::Logger>();

TEST_CASE("Date Time to Text Conversion") {
	datetimeLogger << MESSAGE("Date Time to Text Conversion Test", base::Logger::Severity::Info);
	auto datetime = base::DateTime::DateTime(2020, 5, 20, 17, 15, 45, 100);
	auto datetimeStr = base::DateTime::GetDateTimeAsString(datetime);
	CHECK(datetimeStr == "2020-5-20 17:15:45.100");
	auto datetime2 = base::DateTime::DateTime("2020-5-20 17:15:45.100");
	CHECK(datetime2.GetYear() == 2020);
	CHECK(datetime2.GetMonth() == 5);
	CHECK(datetime2.GetDay() == 20);
	CHECK(datetime2.GetHours() == 17);
	CHECK(datetime2.GetMinutes() == 15);
	CHECK(datetime2.GetSeconds() == 45);
	CHECK(datetime2.GetMilliseconds() == 100);
}

TEST_CASE("Append Date Times") {
	datetimeLogger << MESSAGE("Append Date Times Test", base::Logger::Severity::Info);
	auto datetime = base::DateTime::GetCurrentDateTime();
	auto dateTimeStr1 = base::DateTime::GetCurrentDateTimeAsString();
	auto dateTimeStr2 = base::DateTime::GetDateTimeAsString(datetime);
	base::DateTime dt1(dateTimeStr1);
	base::DateTime dt2(dateTimeStr2);
	CHECK(Catch::Approx((dt1 - dt2).ToMsec()).margin(5) == 0.0);
}