#include "pch.h"
#include "Logger.h"


namespace Util
{
	std::string Logger::s_FilePath = "";

	void Logger::Init(const std::string& filepath)
	{
		s_FilePath = filepath;
		std::ofstream ofs;
		std::stringstream ss;
		ofs.open(filepath, std::ofstream::out | std::ofstream::trunc);

		std::time_t t = std::time(0);
		std::tm now{};
		localtime_s(&now, &t);

		ss << "[LOG " << now.tm_hour << ':' << now.tm_min << ':' << now.tm_sec << "] ";
		ss << "****** Log initalized for NodePlanningEditor ******\n\n\n\n";

		ofs << ss.str();

		ofs.close();
	}

}

