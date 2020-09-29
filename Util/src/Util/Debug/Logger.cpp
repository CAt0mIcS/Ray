#include "pch.h"
#include "Logger.h"

#include "Util/Exceptions.h"

#include <ctime>
#include <fstream>


namespace Util
{
	std::shared_ptr<spdlog::logger> Logger::s_Logger;

	void Logger::Init(const std::string& filename)
	{
		std::ofstream ofs(filename, std::ios_base::trunc);
		ofs.close();

		try
		{
			s_Logger = spdlog::basic_logger_mt("Logger", filename);
			s_Logger->set_level(spdlog::level::trace);
			NPE_LOG("******Log initalized for NodePlanningEditor******\n\n\n");
		}
		catch (const spdlog::spdlog_ex& e)
		{
			std::stringstream ss;
			ss << "Failed to initialize logger\n" << e.what();
			NPE_THROW_EXCEPT_MSG(ss.str().c_str());
		}
	}
}

