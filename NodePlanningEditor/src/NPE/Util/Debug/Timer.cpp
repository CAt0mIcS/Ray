#include "pch.h"
#include "Timer.h"

#include <iostream>


namespace NPE
{
	Timer::Timer(const std::string& text)
	{
		m_Text = text;
		m_Start = std::chrono::steady_clock::now();
	}

	Timer::~Timer()
	{
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_Start).count();
		//NPE_LOG(m_Text + std::to_string(duration) + std::string("us"));
		std::cout << m_Text << duration << "us\n";
	}
}

