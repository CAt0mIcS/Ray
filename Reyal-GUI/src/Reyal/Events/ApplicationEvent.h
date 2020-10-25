#pragma once

#include "Event.h"

#include "Reyal/Core/Vec2.h"


namespace Zeal::Reyal
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(const Size& size)
			: m_Size(size)
		{

		}

		const Size& GetSize() const { return m_Size; }

		RL_DECLARE_TYPE_FN(WindowResizeEvent)

	private:
		Size m_Size;
	};

	class WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(const Point& pos)
			: m_Pos(pos)
		{

		}

		const Point& GetPos() const { return m_Pos; }

		RL_DECLARE_TYPE_FN(WindowMoveEvent)

	private:
		Point m_Pos;
	};

	class SetCursorEvent : public Event
	{
	public:
		SetCursorEvent()
		{
		}

		RL_DECLARE_TYPE_FN(SetCursorEvent)
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent()
		{
		}

		RL_DECLARE_TYPE_FN(WindowCloseEvent)
	};

	class PaintEvent : public Event
	{
	public:
		PaintEvent()
		{

		}

		RL_DECLARE_TYPE_FN(PaintEvent)
	private:
	};
}
