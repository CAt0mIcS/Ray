#ifndef RL_REYALLOGGER_H
#define RL_REYALLOGGER_H
// class 'std::shared_ptr<Zeal::Log::FileLogger>' needs to have dll-interface to be used by clients of class 'Zeal::Reyal::Logger'
#pragma warning(disable : 4251)

#include "RlBase.h"
#include "Reyal/CoreConfig.h"

#include <memory>
#include <string>


#if ZL_ENABLE_LOGGING
#include <ZealLogger/Log.h>


namespace At0::Reyal
{
	class RL_API Logger
	{
	public:
		/// <summary>
		/// Getter for the file logger
		/// </summary>
		/// <returns>The file logger</returns>
		static Log::FileLogger* GetFileLogger() { return s_FileLogger.get(); }

		/// <summary>
		/// Creates and opens all loggers
		/// </summary>
		/// <param name="filepath">Is the path to a log file</param>
		/// <param name="logLvl">Is the log level</param>
		static void Init(const std::string_view filepath, Log::LogLevel logLvl);

	private:
		static std::unique_ptr<Log::FileLogger> s_FileLogger;
	};

}

#endif

/// <summary>
/// Define logging macros here
/// </summary>
#if ZL_ENABLE_LOGGING

	#define ZL_LOG_BEGIN(path, loglevel)::At0::Reyal::Logger::Init(path, loglevel)
	
	#define ZL_LOG_TRACE(msg, ...)		::At0::Reyal::Logger::GetFileLogger()->Trace(msg, __VA_ARGS__)
	#define ZL_LOG_DEBUG(msg, ...)		::At0::Reyal::Logger::GetFileLogger()->Debug(msg, __VA_ARGS__)
	#define ZL_LOG_INFO(msg, ...)		::At0::Reyal::Logger::GetFileLogger()->Info(msg, __VA_ARGS__)
	#define ZL_LOG_WARN(msg, ...)		::At0::Reyal::Logger::GetFileLogger()->Warn(msg, __VA_ARGS__)
	#define ZL_LOG_ERROR(msg, ...)		::At0::Reyal::Logger::GetFileLogger()->Error(msg, __VA_ARGS__)
	#define ZL_LOG_CRITICAL(msg, ...)	::At0::Reyal::Logger::GetFileLogger()->Critical(msg, __VA_ARGS__)

	#define ZL_FL_LOGGER()				::At0::Reyal::Logger::GetFileLogger()

	#define ZL_LOG_END()				::At0::Reyal::Logger::GetFileLogger()->Close();
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

#endif /* RL_REYALLOGGER_H */
