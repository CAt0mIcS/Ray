#pragma once

#define ZL_ENABLE_LOGGING 1


/// <summary>
/// Define logging macros here
/// </summary>
#if ZL_ENABLE_LOGGING
	#include "LoggerClass.h"

	#define ZL_LOG_INIT(path, loglevel)	::Zeal::Log::Logger::Init(path, loglevel)

	#define ZL_LOG_TRACE(msg, ...)		::Zeal::Log::Logger::GetFileLogger()->Trace(msg, __VA_ARGS__)
	#define ZL_LOG_DEBUG(msg, ...)		::Zeal::Log::Logger::GetFileLogger()->Debug(msg, __VA_ARGS__)
	#define ZL_LOG_INFO(msg, ...)		::Zeal::Log::Logger::GetFileLogger()->Info(msg, __VA_ARGS__)
	#define ZL_LOG_WARN(msg, ...)		::Zeal::Log::Logger::GetFileLogger()->Warn(msg, __VA_ARGS__)
	#define ZL_LOG_ERROR(msg, ...)		::Zeal::Log::Logger::GetFileLogger()->Error(msg, __VA_ARGS__)
	#define ZL_LOG_CRITICAL(msg, ...)	::Zeal::Log::Logger::GetFileLogger()->Critical(msg, __VA_ARGS__)
#else
	#define ZL_LOG_INIT(path, loglevel)

	#define ZL_LOG_TRACE(msg, ...)	
	#define ZL_LOG_DEBUG(msg, ...)	
	#define ZL_LOG_INFO(msg, ...)		
	#define ZL_LOG_WARN(msg, ...)		
	#define ZL_LOG_ERROR(msg, ...)	
	#define ZL_LOG_CRITICAL(msg, ...)
#endif