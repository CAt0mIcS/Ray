#pragma once

#include "RlBase.h"

#include <ZealLogger/Log.h>

#include <memory>
#include <string>


#if ZL_ENABLE_LOGGING



namespace Zeal::Reyal
{
	class RL_API Logger
	{
	public:
		/// <summary>
		/// Getter for the file logger
		/// </summary>
		/// <returns>The file logger</returns>
		static std::shared_ptr<Log::FileLogger> GetFileLogger() { return s_FileLogger; }

		/// <summary>
		/// Creates and opens all loggers
		/// </summary>
		/// <param name="filepath">Is the path to a log file</param>
		/// <param name="logLvl">Is the log level</param>
		static void Init(const std::string_view filepath, Log::LogLevel logLvl);

	private:
		static std::shared_ptr<Log::FileLogger> s_FileLogger;
	};

}

#endif

/// <summary>
/// Define logging macros here
/// </summary>
#if ZL_ENABLE_LOGGING

	#define ZL_LOG_BEGIN(path, loglevel)::Zeal::Reyal::Logger::Init(path, loglevel)
	
	#define ZL_LOG_TRACE(msg, ...)		::Zeal::Reyal::Logger::GetFileLogger()->Trace(msg, __VA_ARGS__)
	#define ZL_LOG_DEBUG(msg, ...)		::Zeal::Reyal::Logger::GetFileLogger()->Debug(msg, __VA_ARGS__)
	#define ZL_LOG_INFO(msg, ...)		::Zeal::Reyal::Logger::GetFileLogger()->Info(msg, __VA_ARGS__)
	#define ZL_LOG_WARN(msg, ...)		::Zeal::Reyal::Logger::GetFileLogger()->Warn(msg, __VA_ARGS__)
	#define ZL_LOG_ERROR(msg, ...)		::Zeal::Reyal::Logger::GetFileLogger()->Error(msg, __VA_ARGS__)
	#define ZL_LOG_CRITICAL(msg, ...)	::Zeal::Reyal::Logger::GetFileLogger()->Critical(msg, __VA_ARGS__)

	#define ZL_FL_LOGGER()				::Zeal::Reyal::Logger::GetFileLogger()

	#define ZL_LOG_END()				::Zeal::Reyal::Logger::GetFileLogger()->Close();
#else
	#define ZL_LOG_BEGIN(path, loglevel)
	
	#define ZL_LOG_TRACE(msg, ...)	
	#define ZL_LOG_DEBUG(msg, ...)	
	#define ZL_LOG_INFO(msg, ...)		
	#define ZL_LOG_WARN(msg, ...)		
	#define ZL_LOG_ERROR(msg, ...)	
	#define ZL_LOG_CRITICAL(msg, ...)

	#define ZL_FL_LOGGER()

	#define ZL_LOG_END()
#endif