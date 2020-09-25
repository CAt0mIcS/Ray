#pragma once

#include "UtilBase.h"
#include "pch.h"
#include <fstream>
#include <ctime>
#include <sstream>

#define UTIL_LOG_ACTIVE 1


namespace Util
{

	class UTIL_API Logger
	{
	public:
		static void Init(const std::string& filepath);

		template<typename T>
		static void Log(const T& message);

	private:
		static std::string s_FilePath;
	};

	template<typename T>
	inline void Logger::Log(const T& message)
	{
		std::ofstream stream(s_FilePath, std::ios_base::app);
		std::time_t t = std::time(0);
		std::tm now{};
		localtime_s(&now, &t);
		std::stringstream ss;

		ss << "[LOG " << now.tm_hour << ':' << now.tm_min << ':' << round(now.tm_sec) << "]: " << message << '\n';

		stream << ss.str();
		stream.close();
	}

}


#if UTIL_LOG_ACTIVE
	template<typename T>
	inline void U_UTIL_LOG_U(const T& text)
	{
		Util::Logger::Log((*(T*)&text));
	}

	#define UTIL_LOG(text) U_UTIL_LOG_U(text)
	#define UTIL_LOG_CLEAR(logPath) Util::Logger::Init(logPath)
#else
	template<typename T>
	inline void UTIL_LOG(const T& text) {}
	//#define QRD_LOG(text)
	#define UTIL_LOG_CLEAR()
#endif