#pragma once

#ifdef _DEBUG


#include "../Base.h"
#include <fstream>
#include <ctime>
#include <sstream>


namespace QRD
{

	class QRD_API Logger
	{
	public:
		static void Init(const std::string& filepath);

		template<typename T>
		static void Log(const T& message, const std::string& filepath);
	};

	template<typename T>
	void Logger::Log(const T& message, const std::string& filepath)
	{
		std::ofstream stream(filepath, std::ios_base::app);
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		std::ostringstream ss;
		ss << message;

		stream << '[' << now->tm_hour << ':' << now->tm_min << ':' << round(now->tm_sec) << "]: " << ss.str() << '\n';

		stream.close();
	}

}

#endif

#if QRD_LOG_ACTIVE && defined(_DEBUG)
template<typename T>
inline void QRD_LOG(const T& text)
{
	QRD::Logger::Log((*(T*)&text), "D:\\dev\\Cpp\\Projects\\DatabaseManagementSystem\\log.txt");
}
#define QRD_LOG_CLEAR() QRD::Logger::Init("D:\\dev\\Cpp\\Projects\\DatabaseManagementSystem\\log.txt")
#else
template<typename T>
inline void QRD_LOG(const T& text) {}
//#define QRD_LOG(text)
#define QRD_LOG_CLEAR()
#endif