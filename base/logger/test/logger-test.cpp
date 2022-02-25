#include <catch2/catch.hpp>
#include <logger/logger.h>

TEST_CASE("Logger") {
	auto myLogger = std::make_shared<base::Logger>();
	myLogger << MESSAGE("Logger Test", base::Logger::Severity::Info);
}