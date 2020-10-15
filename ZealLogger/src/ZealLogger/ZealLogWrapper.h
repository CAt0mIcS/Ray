#pragma once

#include <memory>
#include <string>

#include "ZealFileLogger.h"


namespace Zeal::Log
{
	class Logger
	{
	public:
		static std::shared_ptr<FileLogger> Get() { return s_Logger; }

		static void Init(const std::string& filepath)
		{
			if (!s_Logger)
				s_Logger = std::make_shared<FileLogger>();
			else
				s_Logger->Close();

			s_Logger->SetFilePath(filepath);
			s_Logger->Open();
		}

		~Logger()
		{
			s_Logger->Close();
		}

	private:
		static std::shared_ptr<FileLogger> s_Logger;
	};

	inline std::shared_ptr<FileLogger> Logger::s_Logger;
}
