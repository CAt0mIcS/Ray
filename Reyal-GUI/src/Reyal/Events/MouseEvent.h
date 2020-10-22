#pragma once

#include "Event.h"

#include <Util/ZPoint.h>


namespace Zeal::Reyal
{

	enum class MouseButton
	{
		INVALID = 0,
		Left, Middle, Right
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent()
		{
		}

		RL_DECLARE_TYPE_FN(MouseMoveEvent)
	};

	class MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent(const MouseButton btn)
			: m_Button(btn)
		{
		}

		MouseButton GetButton() const { return m_Button; }

		RL_DECLARE_TYPE_FN(MouseButtonPressedEvent)

	private:
		MouseButton m_Button;
	};

	class MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(const MouseButton btn)
			: m_Button(btn)
		{
		}

		MouseButton GetButton() const { return m_Button; }

		RL_DECLARE_TYPE_FN(MouseButtonReleasedEvent)

	private:
		MouseButton m_Button;
	};

	class MouseWheelEvent : public Event
	{
	public:
		int GetDelta() const { return m_Delta; }

	protected:
		MouseWheelEvent(int delta)
			: m_Delta(delta) {}

	private:
		int m_Delta;
	};

	class MouseWheelUpEvent : public MouseWheelEvent
	{
	public:
		MouseWheelUpEvent(int delta)
			: MouseWheelEvent(delta)
		{
		}

		RL_DECLARE_TYPE_FN(MouseWheelUpEvent)
	};

	class MouseWheelDownEvent : public MouseWheelEvent
	{
	public:
		MouseWheelDownEvent(int delta)
			: MouseWheelEvent(delta)
		{
		}

		RL_DECLARE_TYPE_FN(MouseWheelDownEvent)
	};

	class HoverEnterEvent : public Event
	{
	public:
		HoverEnterEvent() = default;

		RL_DECLARE_TYPE_FN(HoverEnterEvent);
	};

	class HoverLeaveEvent : public Event
	{
	public:
		HoverLeaveEvent() = default;

		RL_DECLARE_TYPE_FN(HoverLeaveEvent);
	};
}

