#include <catch2/catch_all.hpp>
#include <assertion/assertion.h>

auto assertionLogger = std::make_shared<base::Logger>();

TEST_CASE("Assertion Test") {
	assertionLogger << MESSAGE("Assertion Test", base::Logger::Severity::Info);
	ASSERT(2 == 2, "2 is equal to 2", base::Logger::Severity::Error);
}