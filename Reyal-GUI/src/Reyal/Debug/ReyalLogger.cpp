#include "rlpch.h"
#include "ReyalLogger.h"

#include <ZealLogger/Log.h>

namespace At0::Reyal
{

#if ZL_ENABLE_LOGGING

	std::unique_ptr<Log::FileLogger> Logger::s_FileLogger = nullptr;

	void Logger::Init(const std::string_view filepath, Log::LogLevel logLvl)
	{
		// TODO: Add assert instead of if-statement
		if (!s_FileLogger)
		{
			s_FileLogger = std::make_unique<Log::FileLogger>(filepath);
			s_FileLogger->SetLogLevel(logLvl);
			s_FileLogger->Open();

			//TODO: Throw proper exception
			if (!s_FileLogger->IsOpen())
				throw std::runtime_error("Failed to open write stream");
		}
	}

#endif
}