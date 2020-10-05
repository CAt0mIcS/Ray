#include "pch.h"
#include "Timer.h"

#include "Util/Exceptions.h"


namespace GUI
{
	unsigned int Timer::s_NextId = 0;

	Timer::Timer(_In_ HWND hWnd, _In_ bool repeat)
		: m_hWnd(hWnd), m_Repeat(repeat), m_Id(s_NextId)
	{
		++s_NextId;
	}
	
	void Timer::Run(_In_ unsigned int time)
	{
		SetTimer(m_hWnd, m_Id, time, nullptr);
	}

	bool Timer::Destroy()
	{
		return KillTimer(GetWindow(), GetId());
	}
}


