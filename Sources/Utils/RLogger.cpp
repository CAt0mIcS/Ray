﻿#include "Rpch.h"
#include "RLogger.h"


namespace At0::Ray
{
	Violent::FileLogger Log::s_FileLogger;
	Violent::ConsoleLogger Log::s_ConsoleLogger;

	void Log::Open(const char* filepath) { s_FileLogger.Open(filepath); }

	void Log::Close()
	{
		s_FileLogger.Close();
		s_ConsoleLogger.Close();
	}

	void Log::SetLogLevel(Violent::LogLevel lvl)
	{
		s_FileLogger.SetLogLevel(lvl);
		s_ConsoleLogger.SetLogLevel(lvl);
	}

	void Log::Flush()
	{
		s_FileLogger.Flush();
		s_ConsoleLogger.Flush();
	}
}  // namespace At0::Ray
