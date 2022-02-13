#pragma once

#include <memory>

#define MESSAGE(X,Y)    std::make_pair(X, Y)

namespace spdlog {
class logger;
namespace level {
enum level_enum;
}
}

namespace base {

class Logger {
public:
	enum Severity : int {
		Trace = 0,
		Debug = 1,
		Info = 2,
		Warn = 3,
		Error = 4,
		Critical = 5,
		Off = 6,
		N_levels = 7
	};

	Logger();

	static spdlog::level::level_enum ConvertSeverity(Severity level);

	static void LogCritical(const char* msg);
	static void LogDebug(const char* msg);
	static void LogError(const char* msg);
	static void LogInfo(const char* msg);
	static void LogTrace(const char* msg);
	static void LogWarn(const char* msg);

	static void SetSeverity(Severity level);
	static Severity GetSeverity();

	friend void operator<<(const std::shared_ptr<Logger>& logger, std::pair<const char*, Severity> msg);

private:
	static Severity m_logLevel;
	static std::shared_ptr<spdlog::logger> m_consoleLogger;
	static std::shared_ptr<spdlog::logger> m_asyncLogger;
};

} // namespace base