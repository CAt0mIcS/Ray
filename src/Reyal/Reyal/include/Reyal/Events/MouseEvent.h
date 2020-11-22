#pragma once

#include "Reyal/Events/Event.h"

#include <../../RlRender/include/RlRender/TypeDefines.h>
#include "Reyal/Widgets/ZWidget.h"


namespace At0::Reyal
{
	enum class MouseButton
	{
		INVALID = 0,
		Left, Right, Middle
	};

	/// <summary>
	/// Converts a mouse button to string
	/// </summary>
	/// <param name="btn">Is the button</param>
	/// <returns>The mouse button name</returns>
	static inline const char* MouseButtonToString(MouseButton btn)
	{
		switch (btn)
		{
		case MouseButton::Left:		return "Left Mouse Button";
		case MouseButton::Right:	return "Right Mouse Button";
		case MouseButton::Middle:	return "Middle Mouse Button";
		}
		return "";
	}


	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(const Point2& oldPos, const Point2& newPos)
			: m_OldPos(oldPos), m_NewPos(newPos)
		{
		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[MouseMoveEvent] New Position: " << m_NewPos;
			return oss.str();
		}

		const Point2& GetPos() const { return m_NewPos; }
		const Point2& GetOldPos() const { return m_OldPos; }

		static EventType GetStaticType() { return EventType::MouseMoveEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		Point2 m_OldPos;
		Point2 m_NewPos;
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
			oss << "[MouseButtonPressedEvent] Button: " << MouseButtonToString(m_Button);
			return oss.str();
		}

		MouseButton GetButton() const { return m_Button; }

		static EventType GetStaticType() { return EventType::MouseButtonPressedEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

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
			oss << "[MouseButtonReleasedEvent] Button: " << MouseButtonToString(m_Button);
			return oss.str();
		}

		MouseButton GetButton() const { return m_Button; }

		static EventType GetStaticType() { return EventType::MouseButtonReleasedEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

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

		static EventType GetStaticType() { return EventType::MouseWheelUpEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
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

		static EventType GetStaticType() { return EventType::MouseWheelDownEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	};

	class MouseWheelLeftEvent : public MouseWheelEvent
	{
	public:
		MouseWheelLeftEvent(int delta)
			: MouseWheelEvent(delta)
		{
		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[MouseWheelLeftEvent] Delta: " << GetDelta();
			return oss.str();
		}

		static EventType GetStaticType() { return EventType::MouseWheelLeftEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	};

	class MouseWheelRightEvent : public MouseWheelEvent
	{
	public:
		MouseWheelRightEvent(int delta)
			: MouseWheelEvent(delta)
		{
		}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[MouseWheelRightEvent] Delta: " << GetDelta();
			return oss.str();
		}

		static EventType GetStaticType() { return EventType::MouseWheelRightEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	};

	class HoverEnterEvent : public Event
	{
	public:
		HoverEnterEvent(Widget* hoverEntered)
			: m_HoverEntered(hoverEntered) {}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[HoverEnterEvent] Widget with name " << m_HoverEntered->GetName() << " gained hover focus";
			return oss.str();
		}

		Widget* GetHoverWidget() const { return m_HoverEntered; }

		static EventType GetStaticType() { return EventType::HoverEnterEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		Widget* m_HoverEntered;
	};

	class HoverLeaveEvent : public Event
	{
	public:
		HoverLeaveEvent(Widget* hoverLeft)
			: m_HoverLeft(hoverLeft) {}

		virtual std::string ToString() const override
		{
			std::ostringstream oss;
			oss << "[HoverLeaveEvent] The Widget with name " << m_HoverLeft->GetName() << " lost hover focus";
			return oss.str();
		}

		static EventType GetStaticType() { return EventType::HoverLeaveEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		Widget* m_HoverLeft;
	};
}

