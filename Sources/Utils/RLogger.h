#pragma once

#include "../RBase.h"
#include "../../Extern/Violent/include/Violent/Violent.h"

#include <string_view>


namespace At0::Ray
{
	class RAY_EXPORT Log
	{
	public:
		static void Open(const char* filepath);
		static void Close();
		static void SetLogLevel(Violent::LogLevel lvl);
		static void Flush();

		template<typename... Args>
		static void Trace(std::string_view msg, Args&&... args)
		{
			s_FileLogger.Trace(msg, std::forward<Args>(args)...);
			s_ConsoleLogger.Trace(msg, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void Debug(std::string_view msg, Args&&... args)
		{
			s_FileLogger.Debug(msg, std::forward<Args>(args)...);
			s_ConsoleLogger.Debug(msg, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void Info(std::string_view msg, Args&&... args)
		{
			s_FileLogger.Info(msg, std::forward<Args>(args)...);
			s_ConsoleLogger.Info(msg, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void Warn(std::string_view msg, Args&&... args)
		{
			s_FileLogger.Warn(msg, std::forward<Args>(args)...);
			s_ConsoleLogger.Warn(msg, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void Error(std::string_view msg, Args&&... args)
		{
			s_FileLogger.Error(msg, std::forward<Args>(args)...);
			s_ConsoleLogger.Error(msg, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void Critical(std::string_view msg, Args&&... args)
		{
			s_FileLogger.Critical(msg, std::forward<Args>(args)...);
			s_ConsoleLogger.Critical(msg, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Trace(std::wstring_view msg, Args&&... args)
		{
			s_FileLogger.Trace(msg, std::forward<Args>(args)...);
			s_ConsoleLogger.Trace(msg, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void Debug(std::wstring_view msg, Args&&... args)
		{
			s_FileLogger.Debug(msg, std::forward<Args>(args)...);
			s_ConsoleLogger.Debug(msg, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void Info(std::wstring_view msg, Args&&... args)
		{
			s_FileLogger.Info(msg, std::forward<Args>(args)...);
			s_ConsoleLogger.Info(msg, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void Warn(std::wstring_view msg, Args&&... args)
		{
			s_FileLogger.Warn(msg, std::forward<Args>(args)...);
			s_ConsoleLogger.Warn(msg, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void Error(std::wstring_view msg, Args&&... args)
		{
			s_FileLogger.Error(msg, std::forward<Args>(args)...);
			s_ConsoleLogger.Error(msg, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void Critical(std::wstring_view msg, Args&&... args)
		{
			s_FileLogger.Critical(msg, std::forward<Args>(args)...);
			s_ConsoleLogger.Critical(msg, std::forward<Args>(args)...);
		}

	private:
		static Violent::FileLogger s_FileLogger;
		static Violent::ConsoleLogger s_ConsoleLogger;
	};
}  // namespace At0::Ray
