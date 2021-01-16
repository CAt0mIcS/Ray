#pragma once


#include <../../RayUtil/include/RayUtil/RCoreConfig.h>
#include <../../RayUtil/include/RayUtil/RException.h>

#include <../../RayLogger/include/RayLogger/RLog.h>

#include "RDBase.h"

#include <string>

#pragma warning(disable : 4251)


namespace At0::Ray::Debug
{
	class RD_API Logger
	{
	public:
		static Log::FileLogger& GetFileLogger() { return s_FileLogger; }

		/// <summary>
		/// Creates and opens all loggers
		/// </summary>
		/// <param name="filepath">Is the path to a log file</param>
		/// <param name="logLvl">Is the log level</param>
		/// <returns>True if initialization was successful</returns>
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

}



#if RAY_ENABLE_LOGGING

namespace At0::Ray::Log
{
	inline void Begin(std::string_view path, Log::LogLevel loglvl)
	{
		if (!::At0::Ray::Debug::Logger::Init(path, loglvl))
			RAY_THROW_RUNTIME("[Logger::Init] Failed");
	}

	inline bool IsOpen()
	{
		return ::At0::Ray::Debug::Logger::GetFileLogger().IsOpen();
	}

	inline void End()
	{
		::At0::Ray::Debug::Logger::End();
	}

	inline void Flush()
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Flush();
	}

	template<typename... Args>
	inline void Trace(std::string_view msg, Args&&... args)
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Trace(msg, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void Debug(std::string_view msg, Args&&... args)
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Debug(msg, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void Info(std::string_view msg, Args&&... args)
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Info(msg, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void Warn(std::string_view msg, Args&&... args)
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Warn(msg, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void Error(std::string_view msg, Args&&... args)
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Error(msg, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void Critical(std::string_view msg, Args&&... args)
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Critical(msg, std::forward<Args>(args)...);
	}

	template<typename... Args>
	inline void Trace(std::wstring_view msg, Args&&... args)
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Trace(msg, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void Debug(std::wstring_view msg, Args&&... args)
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Debug(msg, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void Info(std::wstring_view msg, Args&&... args)
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Info(msg, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void Warning(std::wstring_view msg, Args&&... args)
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Warn(msg, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void Error(std::wstring_view msg, Args&&... args)
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Error(msg, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void Critical(std::wstring_view msg, Args&&... args)
	{
		::At0::Ray::Debug::Logger::GetFileLogger().Critical(msg, std::forward<Args>(args)...);
	}
}





#else
namespace At0::Ray::Log
{
	inline void Begin(std::string_view path, Log::LogLevel loglvl) {}

	inline bool IsOpen() { return false; }

	inline void End() {}

	inline void Flush() {}

	template<typename... Args>
	inline void Trace(std::string_view msg, Args&&... args) {}
	template<typename... Args>
	inline void Debug(std::string_view msg, Args&&... args) {}
	template<typename... Args>
	inline void Info(std::string_view msg, Args&&... args) {}
	template<typename... Args>
	inline void Warn(std::string_view msg, Args&&... args) {}
	template<typename... Args>
	inline void Error(std::string_view msg, Args&&... args) {}
	template<typename... Args>
	inline void Critical(std::string_view msg, Args&&... args) {}

	template<typename... Args>
	inline void LogTrace(std::wstring_view msg, Args&&... args) {}
	template<typename... Args>
	inline void LogDebug(std::wstring_view msg, Args&&... args) {}
	template<typename... Args>
	inline void LogInfo(std::wstring_view msg, Args&&... args) {}
	template<typename... Args>
	inline void LogWarning(std::wstring_view msg, Args&&... args) {}
	template<typename... Args>
	inline void LogError(std::wstring_view msg, Args&&... args) {}
	template<typename... Args>
	inline void LogCritical(std::wstring_view msg, Args&&... args) {}
}
#endif

