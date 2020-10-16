#pragma once

#include "Loggers/FileLogger.h"

#include <memory>
#include <string>


namespace Zeal::Log
{
	class Logger
	{
	public:
		/// <summary>
		/// Getter for the file logger
		/// </summary>
		/// <returns>The file logger</returns>
		static std::shared_ptr<FileLogger> GetFileLogger() { return s_FileLogger; }

		/// <summary>
		/// Creates and opens all loggers
		/// </summary>
		/// <param name="filepath">Is the path to a log file</param>
		/// <param name="logLvl">Is the log level</param>
		static void Init(const std::string& filepath, LogLevel logLvl)
		{
			// TODO: Add assert instead of if-statement
			if (!s_FileLogger)
			{
				s_FileLogger = std::make_shared<FileLogger>(filepath);
				s_FileLogger->SetLogLevel(logLvl);
				s_FileLogger->Open();

				//TODO: Throw proper exception
				if (!s_FileLogger->IsOpen())
					throw std::runtime_error("Failed to open write stream");
			}
		}

	private:
		static std::shared_ptr<FileLogger> s_FileLogger;
	};

	inline std::shared_ptr<FileLogger> Logger::s_FileLogger = nullptr;
}
