#include "Rdpch.h"

#include "RLogger.h"
#include "RAssert.h"



namespace At0::Ray::Debug
{

#if RAY_ENABLE_LOGGING

	Log::FileLogger Logger::s_FileLogger;

	void Logger::Init(const std::string_view filepath, Log::LogLevel logLvl)
	{
		s_FileLogger = Log::FileLogger(filepath);
		s_FileLogger.SetLogLevel(logLvl);
		s_FileLogger.Open();

		RAY_EXPECTS(s_FileLogger.IsOpen());
	}

	void Logger::End()
	{
		s_FileLogger.Close();
	}

#endif
}