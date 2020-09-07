#pragma once


#define NPE_DECLARE_TYPE_FN(type)	static EventType GetStaticType() { return EventType::##type; }\
									virtual EventType GetType() const override { return GetStaticType(); }


namespace GUI
{
	enum class EventType
	{
		INVALID = 0,
		MouseButtonPressedEvent, MouseButtonReleasedEvent, MouseWheelUpEvent, MouseWheelDownEvent, MouseMoveEvent, MouseEnterEvent, MouseLeaveEvent,
		KeyPressedEvent, KeyReleasedEvent, CharEvent,
		AppResizeEvent, AppCloseEvent, AppMoveEvent, AppPaintEvent
	};


	class Event
	{
	public:
		virtual EventType GetType() const = 0;
	};
}



