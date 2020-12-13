#pragma once


#include <RayUtil/RCoreConfig.h>

#if RAY_ENABLE_LOGGING
#include "RDBase.h"

#include <../../RayLogger/include/RayLogger/RLog.h>
#include <string>

#pragma warning(disable : 4251)


namespace At0::Ray::Debug
{
	class RD_API Logger
	{
	public:
		/// <summary>
		/// Getter for the file logger
		/// </summary>
		/// <returns>The file logger</returns>
		static Log::FileLogger& GetFileLogger() { return s_FileLogger; }

		/// <summary>
		/// Creates and opens all loggers
		/// </summary>
		/// <param name="filepath">Is the path to a log file</param>
		/// <param name="logLvl">Is the log level</param>
		static void Init(const std::string_view filepath, Log::LogLevel logLvl);

		/// <summary>
		/// Closes the filestream and deletes the loggers
		/// </summary>
		static void End();

	private:
		static Log::FileLogger s_FileLogger;
	};

}

#endif

/// <summary>
/// Define logging macros here
/// </summary>
#if RAY_ENABLE_LOGGING

#ifdef _MSC_VER
	#define RAY_LOG_BEGIN(path, loglvl)	::At0::Ray::Debug::Logger::Init(path, loglvl)

	#define RAY_LOG_TRACE(msg, ...)		::At0::Ray::Debug::Logger::GetFileLogger().Trace(msg, __VA_ARGS__)
	#define RAY_LOG_DEBUG(msg, ...)		::At0::Ray::Debug::Logger::GetFileLogger().Debug(msg, __VA_ARGS__)
	#define RAY_LOG_INFO(msg, ...)		::At0::Ray::Debug::Logger::GetFileLogger().Info(msg, __VA_ARGS__)
	#define RAY_LOG_WARN(msg, ...)		::At0::Ray::Debug::Logger::GetFileLogger().Warn(msg, __VA_ARGS__)
	#define RAY_LOG_ERROR(msg, ...)		::At0::Ray::Debug::Logger::GetFileLogger().Error(msg, __VA_ARGS__)
	#define RAY_LOG_CRITICAL(msg, ...)	::At0::Ray::Debug::Logger::GetFileLogger().Critical(msg, __VA_ARGS__)

	#define RAY_LOG_IS_OPEN()			::At0::Ray::Debug::Logger::GetFileLogger().IsOpen()

	#define RAY_LOG_END()				::At0::Ray::Debug::Logger::End();
	#define RAY_LOG_FLUSH()				::At0::Ray::Debug::Logger::GetFileLogger().Flush();
#elif defined(__GNUC__)
	#define RAY_LOG_BEGIN(path, loglvl)	::At0::Ray::Debug::Logger::Init(path, loglvl)

	#define RAY_LOG_TRACE(msg, ...)		::At0::Ray::Debug::Logger::GetFileLogger().Trace(msg, ## __VA_ARGS__)
	#define RAY_LOG_DEBUG(msg, ...)		::At0::Ray::Debug::Logger::GetFileLogger().Debug(msg, ## __VA_ARGS__)
	#define RAY_LOG_INFO(msg, ...)		::At0::Ray::Debug::Logger::GetFileLogger().Info(msg, ## __VA_ARGS__)
	#define RAY_LOG_WARN(msg, ...)		::At0::Ray::Debug::Logger::GetFileLogger().Warn(msg, ## __VA_ARGS__)
	#define RAY_LOG_ERROR(msg, ...)		::At0::Ray::Debug::Logger::GetFileLogger().Error(msg, ## __VA_ARGS__)
	#define RAY_LOG_CRITICAL(msg, ...)	::At0::Ray::Debug::Logger::GetFileLogger().Critical(msg, ## __VA_ARGS__)

	#define RAY_LOG_IS_OPEN()			::At0::Ray::Debug::Logger::GetFileLogger().IsOpen()

	#define RAY_LOG_END()				::At0::Ray::Debug::Logger::End();
	#define RAY_LOG_FLUSH()				::At0::Ray::Debug::Logger::GetFileLogger().Flush();
#endif

#else
	#define RAY_LOG_BEGIN(path, loglevel)
	
	#define RAY_LOG_TRACE(msg, ...)	
	#define RAY_LOG_DEBUG(msg, ...)	
	#define RAY_LOG_INFO(msg, ...)		
	#define RAY_LOG_WARN(msg, ...)		
	#define RAY_LOG_ERROR(msg, ...)	
	#define RAY_LOG_CRITICAL(msg, ...)

	#define RAY_LOG_IS_OPEN()

	#define RAY_LOG_END()
	#define RAY_LOG_FLUSH()
#endif

