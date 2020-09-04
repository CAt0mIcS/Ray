#pragma once

#include "pch.h"
#include <fstream>
#include <ctime>
#include <sstream>

#define NPE_LOG_ACTIVE 1


namespace NPE
{

	class Logger
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
		std::stringstream ss;
		ss << message;

		stream << '[' << now->tm_hour << ':' << now->tm_min << ':' << round(now->tm_sec) << "]: " << ss.str() << '\n';

		stream.close();
	}

}


#if NPE_LOG_ACTIVE
	template<typename T>
	inline void NPE_LOG(const T& text)
	{
		NPE::Logger::Log((*(T*)&text), "D:\\dev\\Cpp\\Projects\\NodePlanningEditor\\log.txt");
	}
	#define NPE_LOG_CLEAR() NPE::Logger::Init("D:\\dev\\Cpp\\Projects\\NodePlanningEditor\\log.txt")
#else
	template<typename T>
	inline void NPE_LOG(const T& text) {}
	//#define QRD_LOG(text)
	#define NPE_LOG_CLEAR()
#endif