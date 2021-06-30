#include "Rpch.h"
#include "RLogger.h"


namespace At0::Ray
{
	RAY_EXPORT Violent::FileLogger g_FileLogger;
	RAY_EXPORT Violent::ConsoleLogger g_ConsoleLogger;

	void CLog::Close() { g_ConsoleLogger.Close(); }
	void CLog::SetLogLevel(Violent::LogLevel lvl) { g_ConsoleLogger.SetLogLevel(lvl); }
	void CLog::Flush() { g_ConsoleLogger.Flush(); }

	void FLog::Open(const char* filepath) { g_FileLogger.Open(filepath); }
	void FLog::Close() { g_FileLogger.Close(); }
	void FLog::SetLogLevel(Violent::LogLevel lvl) { g_FileLogger.SetLogLevel(lvl); }
	void FLog::Flush() { g_FileLogger.Flush(); }

	void Log::Open(const char* filepath) { FLog::Open(filepath); }

	void Log::Close()
	{
		FLog::Close();
		CLog::Close();
	}

	void Log::SetLogLevel(Violent::LogLevel lvl)
	{
		FLog::SetLogLevel(lvl);
		CLog::SetLogLevel(lvl);
	}

	void Log::Flush()
	{
		FLog::Flush();
		CLog::Flush();
	}
}  // namespace At0::Ray
