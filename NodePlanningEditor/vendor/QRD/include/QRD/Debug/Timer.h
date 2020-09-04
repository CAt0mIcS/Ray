#pragma once

#ifdef _DEBUG

#include "../Base.h"
#include "Logger.h"

#include <chrono>


namespace QRD
{

	class QRD_API Timer
	{
	public:
		Timer(const std::string& text = "");
		~Timer();

	private:
		std::chrono::steady_clock::time_point m_Start;
		std::string m_Text;
	};

}

#if QRD_TIMER_ACTIVE
	#define TIMER QRD::Timer t(std::string("Timer in file ") + std::string(__FILE__).replace(0, 45, "") + std::string(", function ") + std::string(__FUNCTION__) + std::string(", line ") + std::to_string(__LINE__) + std::string(" took "))
#else
	#define TIMER
#endif
#endif