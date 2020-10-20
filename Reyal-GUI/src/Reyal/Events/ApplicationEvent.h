#pragma once

#include "Event.h"


namespace Zeal::Reyal
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent()
		{

		}

		RL_DECLARE_TYPE_FN(WindowResizeEvent)
	};

	class WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent()
		{

		}

		RL_DECLARE_TYPE_FN(WindowMoveEvent)
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
