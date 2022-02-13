#pragma once

#include <memory>
#include <string/string.h>
#include <logger/logger.h>
#include <iostream>
#include <cassert>

namespace base {

class Logger;

class Assertion {
public:
	static std::shared_ptr<base::Logger> m_logger;
};
}

#define ASSERT(condition, message, severity) \
if (!condition) { \
	std::string msg = base::String::CombineStrings("Assertion failed. ", message); \
	base::Assertion::m_logger << MESSAGE(msg.c_str(), static_cast<base::Logger::Severity>(severity)); \
	assert((condition)); \
}