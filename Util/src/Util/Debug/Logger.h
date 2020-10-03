#pragma once

#include "UtilBase.h"

#define SPDLOG_PRINT_TO_CONSOLE

#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/ostr.h"


namespace Util
{
	class UTIL_API Logger
	{
	public:
		static void Init(const std::string& filename);

		static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}


#define NPE_LOG(...) ::Util::Logger::GetLogger()->info(__VA_ARGS__)
#define NPE_LOG_WARN(...) ::Util::Logger::GetLogger()->warn(__VA_ARGS__)
#define NPE_LOG_ERROR(...) ::Util::Logger::GetLogger()->error(__VA_ARGS__)
#define NPE_LOG_TRACE(...) ::Util::Logger::GetLogger()->trace(__VA_ARGS__)
