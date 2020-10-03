

#ifdef _DEBUG
#include "Logger.h"


namespace QRD
{
	void Logger::Init(const std::string& filepath)
	{
		std::ofstream ofs;
		ofs.open(filepath, std::ofstream::out | std::ofstream::trunc);
		ofs.close();
	}

}

#endif
