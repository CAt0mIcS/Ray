#pragma once

#include "Event.h"
#include "Util/Util.h"

#include "GUI/Util/Timer.h"


namespace GUI
{
	class AppMoveEvent : public Event
	{
	public:
		AppMoveEvent(_In_ const Util::NPoint& pos)
			: m_Pos(pos) {}

		const Util::NPoint& GetPos() const { return m_Pos; }

		NPE_DECLARE_TYPE_FN(AppMoveEvent)

	private:
		Util::NPoint m_Pos;
	};

	class AppResizeEvent : public Event
	{
	public:
		AppResizeEvent(_In_ const Util::NSize& size)
			: m_Size(size) {}

		const Util::NSize& GetSize() const { return m_Size; }

		NPE_DECLARE_TYPE_FN(AppResizeEvent)

	private:
		Util::NSize m_Size;
	};


	/**
	* QUESTION:
	*	Shoud I add "friend class MainWindow" to every event to be able to access hWnd and timerId or make public getters
	*/

	class TimerEvent : public Event
	{
	public:
		TimerEvent(_In_ Timer* timer)
			: m_Timer(timer)
		{
		}

		Timer* GetTimer() const { return m_Timer; }

		NPE_DECLARE_TYPE_FN(TimerEvent)

	private:
		Timer* m_Timer;
	};

	class SetCursorEvent : public Event
	{
	public:
		SetCursorEvent()
		{

		}

		NPE_DECLARE_TYPE_FN(SetCursorEvent)
	};

	class AppCloseEvent : public Event
	{
	public:
		AppCloseEvent()
		{

		}

		NPE_DECLARE_TYPE_FN(AppCloseEvent)
	};

	/**
	* QUESTION:
	*	Should I add SAL defines together like _In_opt_ _Maybenull_ or use only one of them
	*/
	class PaintEvent : public Event
	{
	public:
		PaintEvent(_In_ HDC hDC, _In_opt_ _Maybenull_ RECT* rc)
			: m_hDC(hDC), m_RC(rc)
		{

		}

		HDC GetDC() const { return m_hDC; }
		RECT* GetRect() const { return m_RC; }

		NPE_DECLARE_TYPE_FN(AppPaintEvent)
	private:
		HDC m_hDC;
		RECT* m_RC;
	};
}
