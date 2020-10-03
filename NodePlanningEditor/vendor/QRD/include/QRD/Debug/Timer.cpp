

#ifdef _DEBUG
#include "Timer.h"


namespace QRD
{
	Timer::Timer(const std::string& text)
	{
		m_Text = text;
		m_Start = std::chrono::steady_clock::now();
	}

	Timer::~Timer()
	{
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_Start).count();
		QRD_LOG(m_Text + std::to_string(duration) + std::string("us"));
	}
}

#endif