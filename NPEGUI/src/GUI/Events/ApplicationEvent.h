#pragma once

#include "Event.h"
#include "Util/Util.h"


namespace GUI
{
	class AppMoveEvent : public Event
	{
	public:
		AppMoveEvent(const Util::NPoint& pos)
			: m_Pos(pos) {}

		const Util::NPoint& GetPos() const { return m_Pos; }

		NPE_DECLARE_TYPE_FN(AppMoveEvent)

	private:
		Util::NPoint m_Pos;
	};

	class AppResizeEvent : public Event
	{
	public:
		AppResizeEvent(const Util::NSize& size)
			: m_Size(size) {}

		const Util::NSize& GetSize() const { return m_Size; }

		NPE_DECLARE_TYPE_FN(AppResizeEvent)

	private:
		Util::NSize m_Size;
	};

	class AppCloseEvent : public Event
	{
	public:
		AppCloseEvent()
		{

		}

		NPE_DECLARE_TYPE_FN(AppCloseEvent)
	};

	class PaintEvent : public Event
	{
	public:
		PaintEvent(HDC hDC, RECT* rc)
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
