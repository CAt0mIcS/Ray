#include "rdpch.h"

#include "RLogger.h"
#include "RAssert.h"

#include <RayLogger/RLog.h>



namespace At0::Ray::Debug
{

#if RL_ENABLE_LOGGING

	Log::FileLogger Logger::s_FileLogger;

	void Logger::Init(const std::string_view filepath, Log::LogLevel logLvl)
	{
		s_FileLogger = Log::FileLogger(filepath);
		s_FileLogger.SetLogLevel(logLvl);
		s_FileLogger.Open();

		// TODO: Throw exception here
		RL_EXPECTS(s_FileLogger.IsOpen());
	}

	void Logger::End()
	{
		s_FileLogger.Close();
	}

#endif
}