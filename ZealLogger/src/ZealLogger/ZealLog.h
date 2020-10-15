#pragma once

#define ZL_ENABLE_LOGGING 1


#if ZL_ENABLE_LOGGING

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING


#include "ZealCustomize.h"
#include "ZealFileLogger.h"
#include "ZealLogWrapper.h"


#undef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#endif


#if ZL_ENABLE_LOGGING
	#define ZL_LOG_TRACE(msg, ...)		::Zeal::Log::Logger::Get()->Trace(msg, __VA_ARGS__)
	#define ZL_LOG_DEBUG(msg, ...)		::Zeal::Log::Logger::Get()->Debug(msg, __VA_ARGS__)
	#define ZL_LOG_INFO(msg, ...)		::Zeal::Log::Logger::Get()->Info(msg, __VA_ARGS__)
	#define ZL_LOG_WARN(msg, ...)		::Zeal::Log::Logger::Get()->Warn(msg, __VA_ARGS__)
	#define ZL_LOG_ERROR(msg, ...)		::Zeal::Log::Logger::Get()->Error(msg, __VA_ARGS__)
	#define ZL_LOG_CRITICAL(msg, ...)	::Zeal::Log::Logger::Get()->Critical(msg, __VA_ARGS__)
#else
	#define ZL_LOG_TRACE(msg, ...)	
	#define ZL_LOG_DEBUG(msg, ...)	
	#define ZL_LOG_INFO(msg, ...)		
	#define ZL_LOG_WARN(msg, ...)		
	#define ZL_LOG_ERROR(msg, ...)	
	#define ZL_LOG_CRITICAL(msg, ...)
#endif
