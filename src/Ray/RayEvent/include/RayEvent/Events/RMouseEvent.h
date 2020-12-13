#pragma once

#include "REvent.h"

#include <../../RayRender/include/RayRender/TypeDefines.h>
#include <../../RayUtil/include/RayUtil/RKeyCodes.h>


namespace At0::Ray
{
	class Widget;

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(const Point2& oldPos, const Point2& newPos);
		virtual std::string ToString() const override;

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
		MouseButtonPressedEvent(const MouseButton btn);

		virtual std::string ToString() const override;

		MouseButton GetButton() const { return m_Button; }

		static EventType GetStaticType() { return EventType::MouseButtonPressedEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		MouseButton m_Button;
	};

	class MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(const MouseButton btn);

		virtual std::string ToString() const override;

		MouseButton GetButton() const { return m_Button; }

		static EventType GetStaticType() { return EventType::MouseButtonReleasedEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		MouseButton m_Button;
	};

	class MouseWheelEvent : public Event
	{
	public:
		float GetDelta() const { return m_Delta; }
		virtual ~MouseWheelEvent() = default;
		
	protected:
		MouseWheelEvent(float delta)
			: m_Delta(delta) {}

	private:
		float m_Delta;
	};

	class MouseWheelUpEvent : public MouseWheelEvent
	{
	public:
		MouseWheelUpEvent(float delta);

		virtual std::string ToString() const override;

		static EventType GetStaticType() { return EventType::MouseWheelUpEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	};

	class MouseWheelDownEvent : public MouseWheelEvent
	{
	public:
		MouseWheelDownEvent(float delta);

		virtual std::string ToString() const override;

		static EventType GetStaticType() { return EventType::MouseWheelDownEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	};

	class MouseWheelLeftEvent : public MouseWheelEvent
	{
	public:
		MouseWheelLeftEvent(float delta);

		virtual std::string ToString() const override;

		static EventType GetStaticType() { return EventType::MouseWheelLeftEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	};

	class MouseWheelRightEvent : public MouseWheelEvent
	{
	public:
		MouseWheelRightEvent(float delta);

		virtual std::string ToString() const override;

		static EventType GetStaticType() { return EventType::MouseWheelRightEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }
	};

	class HoverEnterEvent : public Event
	{
	public:
		HoverEnterEvent(Widget* hoverEntered);

		virtual std::string ToString() const override;

		Widget* GetHoverWidget() const { return m_HoverEntered; }

		static EventType GetStaticType() { return EventType::HoverEnterEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		Widget* m_HoverEntered;
	};

	class HoverLeaveEvent : public Event
	{
	public:
		HoverLeaveEvent(Widget* hoverLeft);

		virtual std::string ToString() const override;

		static EventType GetStaticType() { return EventType::HoverLeaveEvent; }
		virtual EventType GetType() const override { return GetStaticType(); }

	private:
		Widget* m_HoverLeft;
	};
}

