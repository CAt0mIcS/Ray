#include "RMouseEvent.h"

#include <sstream>


namespace At0::Ray
{
	/// <summary>
	/// Converts a mouse button to string
	/// </summary>
	/// <param name="btn">Is the button</param>
	/// <returns>The mouse button name</returns>
	static const char* MouseButtonToString(MouseButton btn)
	{
		switch (btn)
		{
		case MouseButton::Left:		return "Left Mouse Button";
		case MouseButton::Right:	return "Right Mouse Button";
		case MouseButton::Middle:	return "Middle Mouse Button";
		}
		return "";
	}


	// -------------------------------------------------------------------
	// MouseMoveEvent
	// -------------------------------------------------------------------
	MouseMoveEvent::MouseMoveEvent(const Point2& oldPos, const Point2& newPos)
		: m_OldPos(oldPos), m_NewPos(newPos)
	{
	}

	std::string MouseMoveEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[MouseMoveEvent] New Position: " << m_NewPos;
		return oss.str();
	}

	// -------------------------------------------------------------------
	// MouseButtonPressedEvent
	// -------------------------------------------------------------------
	MouseButtonPressedEvent::MouseButtonPressedEvent(const MouseButton btn)
		: m_Button(btn)
	{
	}

	std::string MouseButtonPressedEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[MouseButtonPressedEvent] Button: " << MouseButtonToString(m_Button);
		return oss.str();
	}

	// -------------------------------------------------------------------
	// MouseButtonReleasedEvent
	// -------------------------------------------------------------------
	MouseButtonReleasedEvent::MouseButtonReleasedEvent(const MouseButton btn)
		: m_Button(btn)
	{
	}

	std::string MouseButtonReleasedEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[MouseButtonReleasedEvent] Button: " << MouseButtonToString(m_Button);
		return oss.str();
	}

	// -------------------------------------------------------------------
	// MouseWheelUpEvent
	// -------------------------------------------------------------------
	MouseWheelUpEvent::MouseWheelUpEvent(int delta)
		: MouseWheelEvent(delta)
	{
	}

	std::string MouseWheelUpEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[MouseWheelUpEvent] Delta: " << GetDelta();
		return oss.str();
	}

	// -------------------------------------------------------------------
	// MouseWheelDownEvent
	// -------------------------------------------------------------------
	MouseWheelDownEvent::MouseWheelDownEvent(int delta)
		: MouseWheelEvent(delta)
	{
	}

	std::string MouseWheelDownEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[MouseWheelDownEvent] Delta: " << GetDelta();
		return oss.str();
	}

	// -------------------------------------------------------------------
	// MouseWheelLeftEvent
	// -------------------------------------------------------------------
	MouseWheelLeftEvent::MouseWheelLeftEvent(int delta)
		: MouseWheelEvent(delta)
	{
	}

	std::string MouseWheelLeftEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[MouseWheelLeftEvent] Delta: " << GetDelta();
		return oss.str();
	}

	// -------------------------------------------------------------------
	// MouseWheelRightEvent
	// -------------------------------------------------------------------
	MouseWheelRightEvent::MouseWheelRightEvent(int delta)
		: MouseWheelEvent(delta)
	{
	}

	std::string MouseWheelRightEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[MouseWheelRightEvent] Delta: " << GetDelta();
		return oss.str();
	}

	// -------------------------------------------------------------------
	// HoverEnterEvent
	// -------------------------------------------------------------------
	HoverEnterEvent::HoverEnterEvent(Widget* hoverEntered)
		: m_HoverEntered(hoverEntered) 
	{
	}

	std::string HoverEnterEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[HoverEnterEvent]";
		return oss.str();
	}

	// -------------------------------------------------------------------
	// HoverLeaveEvent
	// -------------------------------------------------------------------
	HoverLeaveEvent::HoverLeaveEvent(Widget* hoverLeft)
		: m_HoverLeft(hoverLeft)
	{
	}

	std::string HoverLeaveEvent::ToString() const
	{
		std::ostringstream oss;
		oss << "[HoverLeaveEvent]";
		return oss.str();
	}
}