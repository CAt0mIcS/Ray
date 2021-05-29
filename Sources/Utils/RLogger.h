#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"

#include <source_location>
#include <string_view>

#include <../../Extern/Violent/include/Violent/Violent.h>


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
		struct Trace
		{
			Trace(std::string_view msg, Args&&... args,
				std::source_location loc = std::source_location::current())
			{
				s_FileLogger.Trace(msg, std::forward<Args>(args)...);
				s_ConsoleLogger.Trace(msg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Debug
		{
			Debug(std::string_view msg, Args&&... args,
				std::source_location loc = std::source_location::current())
			{
				s_FileLogger.Debug(msg, std::forward<Args>(args)...);
				s_ConsoleLogger.Debug(msg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Info
		{
			Info(std::string_view msg, Args&&... args,
				std::source_location loc = std::source_location::current())
			{
				s_FileLogger.Info(msg, std::forward<Args>(args)...);
				s_ConsoleLogger.Info(msg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Warn
		{
			Warn(std::string_view msg, Args&&... args,
				std::source_location loc = std::source_location::current())
			{
				s_FileLogger.Warn(msg, std::forward<Args>(args)...);
				s_ConsoleLogger.Warn(msg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Error
		{
			Error(std::string_view msg, Args&&... args,
				std::source_location loc = std::source_location::current())
			{
				s_FileLogger.Error(msg, std::forward<Args>(args)...);
				s_ConsoleLogger.Error(msg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Critical
		{
			Critical(std::string_view msg, Args&&... args,
				std::source_location loc = std::source_location::current())
			{
				s_FileLogger.Critical(msg, std::forward<Args>(args)...);
				s_ConsoleLogger.Critical(msg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		Trace(std::string_view, Args&&...) -> Trace<Args...>;

		template<typename... Args>
		Debug(std::string_view, Args&&...) -> Debug<Args...>;

		template<typename... Args>
		Info(std::string_view, Args&&...) -> Info<Args...>;

		template<typename... Args>
		Warn(std::string_view, Args&&...) -> Warn<Args...>;

		template<typename... Args>
		Error(std::string_view, Args&&...) -> Error<Args...>;

		template<typename... Args>
		Critical(std::string_view, Args&&...) -> Critical<Args...>;

	private:
		static Violent::FileLogger s_FileLogger;
		static Violent::ConsoleLogger s_ConsoleLogger;
	};
}  // namespace At0::Ray
