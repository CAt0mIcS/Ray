#pragma once

#include "RlBase.h"


#define RL_DECLARE_TYPE_FN(type)	static EventType GetStaticType() { return EventType::##type; }\
									virtual EventType GetType() const override { return GetStaticType(); }


namespace Zeal::Reyal
{
	enum class RL_API EventType
	{
		INVALID = 0,
		MouseButtonPressedEvent, MouseButtonReleasedEvent, MouseWheelUpEvent, MouseWheelDownEvent, MouseMoveEvent, 
		HoverEnterEvent, HoverLeaveEvent,
		KeyPressedEvent, KeyReleasedEvent, CharEvent,
		WindowResizeEvent, WindowCloseEvent, WindowMoveEvent, PaintEvent, SetCursorEvent
	};


	class RL_API Event
	{
	public:
		virtual EventType GetType() const = 0;
		virtual std::string ToString() const = 0;

	public:
		bool Handled = false;
	};

}



