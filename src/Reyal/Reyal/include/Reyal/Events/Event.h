#pragma once

namespace At0::Reyal
{
	enum class EventType
	{
		INVALID = 0,
		MouseButtonPressedEvent, MouseButtonReleasedEvent, MouseMoveEvent,
		MouseWheelUpEvent, MouseWheelDownEvent, MouseWheelRightEvent, MouseWheelLeftEvent,
		HoverEnterEvent, HoverLeaveEvent,
		KeyPressedEvent, KeyReleasedEvent, CharEvent,
		WindowResizeEvent, WindowCloseEvent, WindowMoveEvent, PaintEvent, SetCursorEvent,
		FIRST = INVALID, LAST = SetCursorEvent
	};


	class Event
	{
	public:
		virtual EventType GetType() const = 0;
		virtual std::string ToString() const = 0;

		virtual ~Event() = default;
	};

}

