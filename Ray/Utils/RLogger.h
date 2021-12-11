
#pragma once

#include "../RBase.h"

#ifdef _MSC_VER
	#include <source_location>
#else
	#include <experimental/source_location>
#endif

#include <string_view>

#include <../../Extern/Violent/include/Violent/Violent.h>

#ifdef _MSC_VER
	// Don't try to declare a variable with no name
	#pragma warning(disable : 26444)
#endif


namespace At0::Ray
{
#ifdef _MSC_VER
	using SourceLocation = std::source_location;
#else
	using SourceLocation = std::experimental::source_location;
#endif

	RAY_EXPORT extern Violent::FileLogger g_FileLogger;
	RAY_EXPORT extern Violent::ConsoleLogger g_ConsoleLogger;

#ifdef _MSC_VER
	class RAY_EXPORT CLog
	{
		friend class Log;

	public:
#else
	namespace CLog
	{
#endif

#ifdef _MSC_VER
		static void Close();
		static void SetLogLevel(Violent::LogLevel lvl);
		static void Flush();
#else
		void Close();
		void SetLogLevel(Violent::LogLevel lvl);
		void Flush();
#endif

		template<typename... Args>
		struct Trace
		{
			Trace(std::string_view Cmsg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_ConsoleLogger.Trace(Cmsg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Debug
		{
			Debug(std::string_view Cmsg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_ConsoleLogger.Debug(Cmsg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Info
		{
			Info(std::string_view Cmsg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_ConsoleLogger.Info(Cmsg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Warn
		{
			Warn(std::string_view Cmsg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_ConsoleLogger.Warn(Cmsg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Error
		{
			Error(std::string_view Cmsg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_ConsoleLogger.Error(Cmsg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Critical
		{
			Critical(std::string_view Cmsg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_ConsoleLogger.Critical(Cmsg, std::forward<Args>(args)...);
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
#ifdef _MSC_VER
	};
#else
	}
#endif

#ifdef _MSC_VER
	class RAY_EXPORT FLog
	{
		friend class Log;

	public:
#else
	namespace FLog
	{
#endif

#ifdef _MSC_VER
		static void Open(const char* filepath);
		static void Close();
		static void SetLogLevel(Violent::LogLevel lvl);
		static void Flush();
#else
		void Open(const char* filepath);
		void Close();
		void SetLogLevel(Violent::LogLevel lvl);
		void Flush();
#endif

		template<typename... Args>
		struct Trace
		{
			Trace(std::string_view Fmsg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_FileLogger.Trace(Fmsg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Debug
		{
			Debug(std::string_view Fmsg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_FileLogger.Debug(Fmsg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Info
		{
			Info(std::string_view Fmsg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_FileLogger.Info(Fmsg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Warn
		{
			Warn(std::string_view Fmsg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_FileLogger.Warn(Fmsg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Error
		{
			Error(std::string_view Fmsg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_FileLogger.Error(Fmsg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Critical
		{
			Critical(std::string_view Fmsg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_FileLogger.Critical(Fmsg, std::forward<Args>(args)...);
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
#ifdef _MSC_VER
	};
#else
	}
#endif

#ifdef _MSC_VER
	class RAY_EXPORT Log
	{
	public:
#else
	namespace Log
	{
#endif

#ifdef _MSC_VER
		static void Open(const char* filepath);
		static void Close();
		static void SetLogLevel(Violent::LogLevel lvl);
		static void Flush();
#else
		void Open(const char* filepath);
		void Close();
		void SetLogLevel(Violent::LogLevel lvl);
		void Flush();
#endif

		template<typename... Args>
		struct Trace
		{
			Trace(std::string_view msg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_FileLogger.Trace(msg, std::forward<Args>(args)...);
				g_ConsoleLogger.Trace(msg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Debug
		{
			Debug(std::string_view msg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_FileLogger.Debug(msg, std::forward<Args>(args)...);
				g_ConsoleLogger.Debug(msg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Info
		{
			Info(std::string_view msg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_FileLogger.Info(msg, std::forward<Args>(args)...);
				g_ConsoleLogger.Info(msg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Warn
		{
			Warn(std::string_view msg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_FileLogger.Warn(msg, std::forward<Args>(args)...);
				g_ConsoleLogger.Warn(msg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Error
		{
			Error(std::string_view msg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_FileLogger.Error(msg, std::forward<Args>(args)...);
				g_ConsoleLogger.Error(msg, std::forward<Args>(args)...);
			}
		};

		template<typename... Args>
		struct Critical
		{
			Critical(std::string_view msg, Args&&... args,
				SourceLocation loc = SourceLocation::current())
			{
				g_FileLogger.Critical(msg, std::forward<Args>(args)...);
				g_ConsoleLogger.Critical(msg, std::forward<Args>(args)...);
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
	};
}  // namespace At0::Ray
