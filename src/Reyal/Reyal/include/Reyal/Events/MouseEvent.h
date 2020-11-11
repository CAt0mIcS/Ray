#ifndef RL_MOUSEEVENT_H
#define RL_MOUSEEVENT_H

#include "Reyal/Events/Event.h"

#include <../../RlRender/include/RlRender/TypeDefines.h>
#include "Reyal/Widgets/ZWidget.h"


namespace At0::Reyal
{

	enum class MouseButton
	{
		INVALID = 0,
		Left, Middle, Right
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(const Point2& oldPos, const Point2& newPos)
			: m_OldPos(oldPos), m_NewPos(newPos)
		{
		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[MouseMoveEvent] New Position: " << m_NewPos;
			return oss.str();
		}

		const Point2& GetPos() const { return m_NewPos; }
		const Point2& GetOldPos() const { return m_OldPos; }

		RL_DECLARE_TYPE_FN(MouseMoveEvent)

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

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[MouseButtonPressedEvent] Button: " << (int)m_Button;
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

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[MouseButtonReleasedEvent] Button: " << (int)m_Button;
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

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[MouseWheelUpEvent] Delta: " << GetDelta();
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

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[MouseWheelDownEvent] Delta: " << GetDelta();
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(MouseWheelDownEvent)
	};

	class HoverEnterEvent : public Event
	{
	public:
		HoverEnterEvent(_In_ Widget* hoverEntered)
			: m_HoverEntered(hoverEntered) {}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[HoverEnterEvent] Widget with name " << m_HoverEntered->GetName() << " gained hover focus";
			return oss.str();
		}

		Widget* GetHoverWidget() const { return m_HoverEntered; }

		RL_DECLARE_TYPE_FN(HoverEnterEvent);

	private:
		Widget* m_HoverEntered;
	};

	class HoverLeaveEvent : public Event
	{
	public:
		HoverLeaveEvent(_In_ Widget* hoverLeft)
			: m_HoverLeft(hoverLeft) {}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[HoverLeaveEvent] The Widget with name " << m_HoverLeft->GetName() << " lost hover focus";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(HoverLeaveEvent);

	private:
		Widget* m_HoverLeft;
	};
}

#endif // RL_MOUSEEVENT_H