#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks-inl.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "logger/logger.h"

namespace base {

Logger::Severity Logger::m_logLevel = base::Logger::Severity::Error;
std::shared_ptr<spdlog::logger> Logger::m_consoleLogger = spdlog::stdout_color_mt("console");
std::shared_ptr<spdlog::logger> Logger::m_asyncLogger = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/async_log.txt");

Logger::Logger() {
	spdlog::set_pattern("[%^+++%$] [%H:%M:%S %z] [thread %t] %v");
	spdlog::set_level(ConvertSeverity(m_logLevel));
}

spdlog::level::level_enum 
Logger::ConvertSeverity(Severity level) {
	switch (level) {
	case Severity::Trace: return spdlog::level::trace; break;
	case Severity::Debug: return spdlog::level::debug; break;
	case Severity::Info: return spdlog::level::info; break;
	case Severity::Warn: return spdlog::level::warn; break;
	case Severity::Error: return spdlog::level::err; break;
	case Severity::Critical: return spdlog::level::critical; break;
	case Severity::Off: return spdlog::level::off; break;
	case Severity::N_levels: return spdlog::level::n_levels; break;
	default: return spdlog::level::err; break;
	}
}

void 
Logger::LogCritical(const char* msg) {
	m_consoleLogger->critical(msg);
	m_asyncLogger->critical(msg);
}

void 
Logger::LogDebug(const char* msg) {
	m_consoleLogger->debug(msg);
	m_asyncLogger->debug(msg);
}

void 
Logger::LogError(const char* msg) {
	m_consoleLogger->error(msg);
	m_asyncLogger->error(msg);
}

void 
Logger::LogInfo(const char* msg) {
	m_consoleLogger->info(msg);
	m_asyncLogger->info(msg);
}

void 
Logger::LogTrace(const char* msg) {
	m_consoleLogger->trace(msg);
	m_asyncLogger->trace(msg);
}

void 
Logger::LogWarn(const char* msg) {
	m_consoleLogger->warn(msg);
	m_asyncLogger->warn(msg);
}

void 
Logger::SetSeverity(Severity level) {
	m_logLevel = level;
	spdlog::set_level(ConvertSeverity(m_logLevel));
}

Logger::Severity 
Logger::GetSeverity() {
	return m_logLevel;
}

void operator<<(const std::shared_ptr<Logger>& logger, std::pair<const char*, base::Logger::Severity> msg) {
	auto level = spdlog::get_level();
	bool shouldLog = spdlog::should_log(Logger::ConvertSeverity(msg.second));
	switch (msg.second) {
	case Logger::Severity::Critical: logger->LogCritical(msg.first); break;
	case Logger::Severity::Debug: logger->LogDebug(msg.first); break;
	case Logger::Severity::Error: logger->LogError(msg.first); break;
	case Logger::Severity::Info: logger->LogInfo(msg.first); break;
	case Logger::Severity::N_levels: break;
	case Logger::Severity::Off: break;
	case Logger::Severity::Trace: logger->LogTrace(msg.first); break;
	case Logger::Severity::Warn: logger->LogWarn(msg.first); break;
	default: break;
	}
}

} 