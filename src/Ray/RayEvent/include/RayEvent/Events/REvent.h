#pragma once

#include <string>


namespace At0::Ray
{
	enum class EventType
	{
		INVALID = 0,
		MouseButtonPressedEvent, MouseButtonReleasedEvent, MouseMoveEvent,
		MouseWheelUpEvent, MouseWheelDownEvent, MouseWheelRightEvent, MouseWheelLeftEvent,
		HoverEnterEvent, HoverLeaveEvent,
		KeyPressedEvent, KeyReleasedEvent, CharEvent,
		WindowResizeEvent, WindowCloseEvent, WindowMoveEvent, PaintEvent,
		FIRST = INVALID, LAST = PaintEvent
	};


	class Event
	{
	public:
		virtual EventType GetType() const = 0;

		// C++20 (virtual constexpr)
		virtual /*constexpr */std::string ToString() const = 0;

		virtual ~Event() = default;
	};

}

