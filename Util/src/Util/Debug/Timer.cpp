#include "pch.h"
#include "Timer.h"


namespace Util
{
	Timer::Timer(const std::string& text)
	{
		m_Text = text;
		m_Start = std::chrono::steady_clock::now();
	}

	Timer::~Timer()
	{
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_Start).count();
		//Util_LOG(m_Text + std::to_string(duration) + std::string("us"));
		//std::cout << m_Text << duration << "us\n";
#if UTIL_LOG_TIMER
		NPE_LOG("{0}{1}us", m_Text, duration);
#else
		std::cout << m_Text << duration << "us\n";
#endif
	}
}

