#include <catch2/catch.hpp>
#include <assertion/assertion.h>

TEST_CASE("ASSERT") {
	ASSERT(2 == 2, "2 is equal to 2", base::Logger::Severity::Error);
}