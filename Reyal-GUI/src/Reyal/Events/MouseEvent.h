#pragma once

#include "Event.h"

#include "Reyal/Core/Vec2.h"


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
		MouseMoveEvent(const Point& pos)
			: m_Pos(pos)
		{
		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[MouseMoveEvent] New Position: " << m_Pos;
			return oss.str();
		}

		const Point& GetPos() const { return m_Pos; }

		RL_DECLARE_TYPE_FN(MouseMoveEvent)

	private:
		Point m_Pos;
	};

	class MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent(const MouseButton btn)
			: m_Button(btn)
		{
		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[MouseButtonPressedEvent] Button: " << (int)m_Button;
			return oss.str();
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

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[MouseButtonReleasedEvent] Button: " << (int)m_Button;
			return oss.str();
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

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[MouseWheelUpEvent] Delta: " << GetDelta();
			return oss.str();
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

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[MouseWheelDownEvent] Delta: " << GetDelta();
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(MouseWheelDownEvent)
	};

	class HoverEnterEvent : public Event
	{
	public:
		HoverEnterEvent() = default;

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[HoverEnterEvent]";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(HoverEnterEvent);
	};

	class HoverLeaveEvent : public Event
	{
	public:
		HoverLeaveEvent() = default;

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[HoverLeaveEvent]";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(HoverLeaveEvent);
	};
}

