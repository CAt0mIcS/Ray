#pragma once

#include "../Core/RMath.h"
#include "../Core/RKeyCodes.h"


namespace At0::Ray
{
	class Widget;

	class MouseButtonPressedEvent
	{
	public:
		MouseButtonPressedEvent(MouseButton btn, Widget* widget) : m_Button(btn), m_Widget(widget)
		{
		}

		MouseButton GetKey() const { return m_Button; }
		Widget* GetWidget() { return m_Widget; }

		bool Handled = false;

	private:
		MouseButton m_Button;
		Widget* m_Widget;
	};

	class MouseButtonReleasedEvent
	{
	public:
		MouseButtonReleasedEvent(MouseButton btn, Widget* widget) : m_Button(btn), m_Widget(widget)
		{
		}

		MouseButton GetKey() const { return m_Button; }
		Widget* GetWidget() { return m_Widget; }

		bool Handled = false;

	private:
		MouseButton m_Button;
		Widget* m_Widget;
	};

	class MouseMovedEvent
	{
	public:
		MouseMovedEvent(Float2 newPos, Int2 delta) : m_Pos(newPos), m_Delta(delta) {}

		Float2 GetPos() const { return m_Pos; }
		Int2 GetDelta() const { return m_Delta; }

		bool Handled = false;

	private:
		Float2 m_Pos;
		Int2 m_Delta;
	};

	class ScrollEvent
	{
	public:
		ScrollEvent(Int2 offset) : m_Offset(offset) {}

		Int2 GetOffset() const { return m_Offset; }

		bool Handled = false;

	private:
		Int2 m_Offset;
	};

	class ScrollUpEvent : public ScrollEvent
	{
	public:
		using ScrollEvent::ScrollEvent;
	};

	class ScrollDownEvent : public ScrollEvent
	{
	public:
		using ScrollEvent::ScrollEvent;
	};

	class ScrollLeftEvent : public ScrollEvent
	{
	public:
		using ScrollEvent::ScrollEvent;
	};

	class ScrollRightEvent : public ScrollEvent
	{
	public:
		using ScrollEvent::ScrollEvent;
	};

	class HoverEnterEvent
	{
	public:
		HoverEnterEvent(Widget* widget) : m_Widget(widget) {}

		Widget* GetWidget() { return m_Widget; }

	private:
		Widget* m_Widget;
	};

	class HoverLeaveEvent
	{
	public:
		HoverLeaveEvent(Widget* widget) : m_Widget(widget) {}

		Widget* GetWidget() { return m_Widget; }

	private:
		Widget* m_Widget;
	};

}  // namespace At0::Ray
