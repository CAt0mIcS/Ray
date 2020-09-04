#include "pch.h"
#include "Logger.h"


namespace NPE
{
	void Logger::Init(const std::string& filepath)
	{
		std::ofstream ofs;
		ofs.open(filepath, std::ofstream::out | std::ofstream::trunc);
		ofs.close();
	}

}

