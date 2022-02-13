#include <catch2/catch.hpp>
#include <string/string.h>

TEST_CASE("Split String") {
	auto CheckStrings = [&] (const auto& splits, const std::string& delimiter) {
		REQUIRE(splits.size() == 5);
		CHECK(splits[0] == "Test");
		CHECK(splits[1] == "String");
		CHECK(splits[2] == "With");
		CHECK(splits[3] == delimiter);
		CHECK(splits[4] == "Delimiters");
	};
	std::vector<std::string> splits;
	splits = base::String::SplitString("Test String With Space Delimiters", " ");
	CheckStrings(splits, "Space");
	splits = base::String::SplitString("Test/String/With/Slash/Delimiters", "/");
	CheckStrings(splits, "Slash");
	splits = base::String::SplitString("Test\\String\\With\\BackSlash\\Delimiters", "\\");
	CheckStrings(splits, "BackSlash");
	splits = base::String::SplitString("TestString", "Delimiter");
	REQUIRE(splits.size() == 1);
	CHECK(splits[0] == "TestString");
	splits = base::String::SplitString("", "Delimiter");
	REQUIRE(splits.size() == 1);
	CHECK(splits[0] == "");
}

TEST_CASE("Substring Existance") {
	std::string string1 = "TestString";
	std::string string2 = "String";
	const char* cString1 = "TestString";
	const char* cString2 = "String";
	size_t pos = 0;

	CHECK(base::String::SubstringExists(string1, string1, pos) == true);
	CHECK(base::String::SubstringExists(string1, string2, pos) == true);
	CHECK(base::String::SubstringExists(string1, cString1, pos) == true);
	CHECK(base::String::SubstringExists(string1, cString2, pos) == true);

	CHECK(base::String::SubstringExists(string2, string1, pos) == false);
	CHECK(base::String::SubstringExists(string2, string2, pos) == true);
	CHECK(base::String::SubstringExists(string2, cString1, pos) == false);
	CHECK(base::String::SubstringExists(string2, cString2, pos) == true);

	CHECK(base::String::SubstringExists(cString1, string1, pos) == true);
	CHECK(base::String::SubstringExists(cString1, string2, pos) == true);
	CHECK(base::String::SubstringExists(cString1, cString1, pos) == true);
	CHECK(base::String::SubstringExists(cString1, cString2, pos) == true);

	CHECK(base::String::SubstringExists(cString2, string1, pos) == false);
	CHECK(base::String::SubstringExists(cString2, string2, pos) == true);
	CHECK(base::String::SubstringExists(cString2, cString1, pos) == false);
	CHECK(base::String::SubstringExists(cString2, cString2, pos) == true);
}

TEST_CASE("Combine Strings") {
	std::string string1 = "Test";
	std::string string2 = "String";
	const char* cString1 = "Test";
	const char* cString2 = "String";
	CHECK(base::String::CombineStrings(string1, string2) == "TestString");
	CHECK(base::String::CombineStrings(string1, cString2) == "TestString");
	CHECK(base::String::CombineStrings(cString1, string2) == "TestString");
	CHECK(base::String::CombineStrings(cString1, cString2) == "TestString");
}

TEST_CASE("Combine Numericals to Strings - Strings to Numericals") {
	//// CONVERT NUMERICALS TO STRINGS ////
	auto int1 = base::String::ConvertNumericalToString(std::numeric_limits<int>::min(), 0);
	auto int2 = base::String::ConvertNumericalToString(std::numeric_limits<int>::max(), 0);
	auto int3 = base::String::ConvertNumericalToString(std::numeric_limits<int>::lowest(), 0);
	CHECK(int1 == "-2147483648");
	CHECK(int2 == "2147483647");
	CHECK(int3 == "-2147483648");

	auto double1 = base::String::ConvertNumericalToString(std::numeric_limits<double>::min(), 2);
	auto double2 = base::String::ConvertNumericalToString(std::numeric_limits<double>::max(), 2);
	auto double3 = base::String::ConvertNumericalToString(std::numeric_limits<double>::lowest(), 2);
	CHECK(double1 == "0.00");
	CHECK(double2 == "179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.00");
	CHECK(double3 == "-179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.00");

	auto float1 = base::String::ConvertNumericalToString(std::numeric_limits<float>::min(), 2);
	auto float2 = base::String::ConvertNumericalToString(std::numeric_limits<float>::max(), 2);
	auto float3 = base::String::ConvertNumericalToString(std::numeric_limits<float>::lowest(), 2);
	CHECK(float1 == "0.00");
	CHECK(float2 == "340282346638528859811704183484516925440.00");
	CHECK(float3 == "-340282346638528859811704183484516925440.00");

	auto long1 = base::String::ConvertNumericalToString(std::numeric_limits<long>::min(), 2);
	auto long2 = base::String::ConvertNumericalToString(std::numeric_limits<long>::max(), 2);
	auto long3 = base::String::ConvertNumericalToString(std::numeric_limits<long>::lowest(), 2);
	CHECK(long1 == "-2147483648");
	CHECK(long2 == "2147483647");
	CHECK(long3 == "-2147483648");

	auto longdouble1 = base::String::ConvertNumericalToString(std::numeric_limits<long double>::min(), 2);
	auto longdouble2 = base::String::ConvertNumericalToString(std::numeric_limits<long double>::max(), 2);
	auto longdouble3 = base::String::ConvertNumericalToString(std::numeric_limits<long double>::lowest(), 2);
	CHECK(longdouble1 == "0.00");
	CHECK(longdouble2 == "179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.00");
	CHECK(longdouble3 == "-179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.00");

	auto longlong1 = base::String::ConvertNumericalToString(std::numeric_limits<long long>::min(), 2);
	auto longlong2 = base::String::ConvertNumericalToString(std::numeric_limits<long long>::max(), 2);
	auto longlong3 = base::String::ConvertNumericalToString(std::numeric_limits<long long>::lowest(), 2);
	CHECK(longlong1 == "-9223372036854775808");
	CHECK(longlong2 == "9223372036854775807");
	CHECK(longlong3 == "-9223372036854775808");

	auto unsignedlong1 = base::String::ConvertNumericalToString(std::numeric_limits<unsigned long>::min(), 2);
	auto unsignedlong2 = base::String::ConvertNumericalToString(std::numeric_limits<unsigned long>::max(), 2);
	auto unsignedlong3 = base::String::ConvertNumericalToString(std::numeric_limits<unsigned long>::lowest(), 2);
	CHECK(unsignedlong1 == "0");
	CHECK(unsignedlong2 == "4294967295");
	CHECK(unsignedlong3 == "0");

	auto unsignedlonglong1 = base::String::ConvertNumericalToString(std::numeric_limits<unsigned long long>::min(), 2);
	auto unsignedlonglong2 = base::String::ConvertNumericalToString(std::numeric_limits<unsigned long long>::max(), 2);
	auto unsignedlonglong3 = base::String::ConvertNumericalToString(std::numeric_limits<unsigned long long>::lowest(), 2);
	CHECK(unsignedlonglong1 == "0");
	CHECK(unsignedlonglong2 == "18446744073709551615");
	CHECK(unsignedlonglong3 == "0");

	//// CONVERT STRINGS TO NUMERICALS ////
	CHECK(base::String::ConvertStringToNumerical<int>(int1.c_str()) == std::numeric_limits<int>::min());
	CHECK(base::String::ConvertStringToNumerical<int>(int2.c_str()) == std::numeric_limits<int>::max());
	CHECK(base::String::ConvertStringToNumerical<int>(int3.c_str()) == std::numeric_limits<int>::lowest());

	CHECK(Approx(base::String::ConvertStringToNumerical<double>(double1.c_str())).margin(1e-307) == std::numeric_limits<double>::min());
	CHECK(Approx(base::String::ConvertStringToNumerical<double>(double2.c_str())).margin(1e-307) == std::numeric_limits<double>::max());
	CHECK(Approx(base::String::ConvertStringToNumerical<double>(double3.c_str())).margin(1e-307) == std::numeric_limits<double>::lowest());

	CHECK(Approx(base::String::ConvertStringToNumerical<float>(float1.c_str())).margin(1e-37) == std::numeric_limits<float>::min());
	CHECK(Approx(base::String::ConvertStringToNumerical<float>(float2.c_str())).margin(1e-37) == std::numeric_limits<float>::max());
	CHECK(Approx(base::String::ConvertStringToNumerical<float>(float3.c_str())).margin(1e-37) == std::numeric_limits<float>::lowest());

	CHECK(base::String::ConvertStringToNumerical<long>(long1.c_str()) == std::numeric_limits<long>::min());
	CHECK(base::String::ConvertStringToNumerical<long>(long2.c_str()) == std::numeric_limits<long>::max());
	CHECK(base::String::ConvertStringToNumerical<long>(long3.c_str()) == std::numeric_limits<long>::lowest());

	CHECK(Approx(base::String::ConvertStringToNumerical<long double>(longdouble1.c_str())).margin(1e-307) == std::numeric_limits<long double>::min());
	CHECK(Approx(base::String::ConvertStringToNumerical<long double>(longdouble2.c_str())).margin(1e-307) == std::numeric_limits<long double>::max());
	CHECK(Approx(base::String::ConvertStringToNumerical<long double>(longdouble3.c_str())).margin(1e-307) == std::numeric_limits<long double>::lowest());

	CHECK(base::String::ConvertStringToNumerical<long long>(longlong1.c_str()) == std::numeric_limits<long long>::min());
	CHECK(base::String::ConvertStringToNumerical<long long>(longlong2.c_str()) == std::numeric_limits<long long>::max());
	CHECK(base::String::ConvertStringToNumerical<long long>(longlong3.c_str()) == std::numeric_limits<long long>::lowest());

	CHECK(base::String::ConvertStringToNumerical<unsigned long>(unsignedlong1.c_str()) == std::numeric_limits<unsigned long>::min());
	CHECK(base::String::ConvertStringToNumerical<unsigned long>(unsignedlong2.c_str()) == std::numeric_limits<unsigned long>::max());
	CHECK(base::String::ConvertStringToNumerical<unsigned long>(unsignedlong3.c_str()) == std::numeric_limits<unsigned long>::lowest());

	CHECK(base::String::ConvertStringToNumerical<unsigned long long>(unsignedlonglong1.c_str()) == std::numeric_limits<unsigned long long>::min());
	CHECK(base::String::ConvertStringToNumerical<unsigned long long>(unsignedlonglong2.c_str()) == std::numeric_limits<unsigned long long>::max());
	CHECK(base::String::ConvertStringToNumerical<unsigned long long>(unsignedlonglong3.c_str()) == std::numeric_limits<unsigned long long>::lowest());
}

TEST_CASE("Upper / Lower Cases") {
	std::string test1 = "Test Cases With Space Delimiters";
	const char* test2 = "Test Cases With Space Delimiters";
	CHECK(base::String::ToLowerCase(test1) == "test cases with space delimiters");
	CHECK(base::String::ToUpperCase(test1) == "TEST CASES WITH SPACE DELIMITERS");
	CHECK(base::String::ToLowerCase(test2) == "test cases with space delimiters");
	CHECK(base::String::ToUpperCase(test2) == "TEST CASES WITH SPACE DELIMITERS");
}

TEST_CASE("Replace Strings") {
	std::string test1 = "D:\\Workspace\\Test\\Project1";
	const char* test2 = "D:\\Workspace\\Test\\Project2";
	auto test1Fixed = base::String::ReplaceAll(test1, "\\", "/");
	auto test2Fixed = base::String::ReplaceAll(test2, "\\", "/");
	CHECK(test1Fixed == "D:/Workspace/Test/Project1");
	CHECK(test2Fixed == "D:/Workspace/Test/Project2");
	auto test1Ws = base::String::ReplaceAll(test1Fixed, "Workspace", "WS");
	auto test2Ws = base::String::ReplaceAll(test2Fixed, "Workspace", "WS");
	CHECK(test1Ws == "D:/WS/Test/Project1");
	CHECK(test2Ws == "D:/WS/Test/Project2");
}