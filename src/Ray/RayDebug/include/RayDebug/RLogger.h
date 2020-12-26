#pragma once


#include <../../RayUtil/include/RayUtil/RCoreConfig.h>

#if RAY_ENABLE_LOGGING
#include "RDBase.h"

#include <../../RayLogger/include/RayLogger/RLog.h>
#include <string>

#pragma warning(disable : 4251)


namespace At0::Ray::Debug
{
	class Logger
	{
	public:
		static Log::FileLogger& GetFileLogger() { return s_FileLogger; }

		/// <summary>
		/// Creates and opens all loggers
		/// </summary>
		/// <param name="filepath">Is the path to a log file</param>
		/// <param name="logLvl">Is the log level</param>
		/// <returns>True if initialisation was successful</returns>
		static bool Init(std::string_view filepath, Log::LogLevel logLvl)
		{
			s_FileLogger = Log::FileLogger(filepath);
			s_FileLogger.SetLogLevel(logLvl);
			s_FileLogger.Open();

			return s_FileLogger.IsOpen();
		}

		/// <summary>
		/// Closes the filestream and deletes the loggers
		/// </summary>
		static void End()
		{
			s_FileLogger.Close();
		}

	private:
		static Log::FileLogger s_FileLogger;
	};

	inline Log::FileLogger Logger::s_FileLogger;

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

