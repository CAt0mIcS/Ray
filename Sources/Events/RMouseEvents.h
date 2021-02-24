#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"
#include "../Core/RKeyCodes.h"


namespace At0::Ray
{
	class MouseButtonPressedEvent
	{
	public:
		MouseButtonPressedEvent(MouseButton btn) : m_Button(btn) {}

		MouseButton GetKey() const { return m_Button; }

	private:
		MouseButton m_Button;
	};

	class MouseButtonReleasedEvent
	{
	public:
		MouseButtonReleasedEvent(MouseButton btn) : m_Button(btn) {}

		MouseButton GetKey() const { return m_Button; }

	private:
		MouseButton m_Button;
	};

	class MouseMovedEvent
	{
	public:
		MouseMovedEvent(Float2 newPos, Int2 delta) : m_Pos(newPos), m_Delta(delta) {}

		Float2 GetPosition() const { return m_Pos; }
		Int2 GetDelta() const { return m_Delta; }

	private:
		Float2 m_Pos;
		Int2 m_Delta;
	};

	class ScrollEvent
	{
	public:
		ScrollEvent(Int2 offset) : m_Offset(offset) {}

		Int2 GetOffset() const { return m_Offset; }

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
}  // namespace At0::Ray
