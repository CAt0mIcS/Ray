#include "pch.h"
#include "Timer.h"

#include "Util/Exceptions.h"


namespace GUI
{
	unsigned int Timer::s_NextId = 0;

	Timer::Timer(HWND hWnd, bool repeat)
		: m_hWnd(hWnd), m_Repeat(repeat), m_Id(s_NextId)
	{
		++s_NextId;
	}
	
	void Timer::Run(unsigned int time)
	{
		SetTimer(m_hWnd, m_Id, time, nullptr);
	}

	void Timer::Destroy()
	{
		if (!KillTimer(GetWindow(), GetId()))
			NPE_THROW_WND_EXCEPT(GetLastError());
	}
}


