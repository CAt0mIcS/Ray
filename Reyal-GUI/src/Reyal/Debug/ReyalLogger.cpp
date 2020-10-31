#include "rlpch.h"
#include "ReyalLogger.h"

#include <ZealLogger/Log.h>

#include "Reyal/Debug/RlAssert.h"


namespace At0::Reyal
{

#if ZL_ENABLE_LOGGING

	std::unique_ptr<Log::FileLogger> Logger::s_FileLogger = nullptr;

	void Logger::Init(const std::string_view filepath, Log::LogLevel logLvl)
	{
		RL_ASSERT(!s_FileLogger, "FileLogger already initialized");
		s_FileLogger = std::make_unique<Log::FileLogger>(filepath);
		s_FileLogger->SetLogLevel(logLvl);
		s_FileLogger->Open();

		// TODO: Throw exception here
		RL_EXPECTS(s_FileLogger->IsOpen());
	}

#endif
}