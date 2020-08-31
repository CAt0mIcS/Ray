#include "Mouse.h"



namespace NPE
{
	Mouse::Mouse()
		: m_Pos{ 0, 0 }, m_IsLeftPressed(false), m_IsRightPressed(false), m_IsMiddlePressed(false), m_WheelDeltaCarry(0)
	{
	}

	Mouse::MouseEvent Mouse::GetEvent()
	{
		if (m_Queue.size() > 0u)
		{
			auto e = m_Queue.front();
			m_Queue.pop();
			return e;
		}
		return MouseEvent();
	}

	void Mouse::OnLButtonDown()
	{
		m_IsLeftPressed = true;
		m_Queue.push(MouseEvent(MouseEvent::MouseEventType::LPress, *this));
		TrimBuffer();
	}

	void Mouse::OnLButtonUp()
	{
		m_IsLeftPressed = false;
		m_Queue.push(MouseEvent(MouseEvent::MouseEventType::LRelease, *this));
		TrimBuffer();
	}

	void Mouse::OnRButtonDown()
	{
		m_IsRightPressed = false;
		m_Queue.push(MouseEvent(MouseEvent::MouseEventType::RPress, *this));
		TrimBuffer();
	}

	void Mouse::OnRButtonUp()
	{
		m_IsRightPressed = false;
		m_Queue.push(MouseEvent(MouseEvent::MouseEventType::RRelease, *this));
		TrimBuffer();
	}

	void Mouse::OnMButtonDown()
	{
		m_IsMiddlePressed = true;
		m_Queue.push(MouseEvent(MouseEvent::MouseEventType::MWheelPress, *this));
		TrimBuffer();
	}

	void Mouse::OnMButtonUp()
	{
		m_IsMiddlePressed = false;
		m_Queue.push(MouseEvent(MouseEvent::MouseEventType::MWheelRelease, *this));
		TrimBuffer();
	}

	void Mouse::OnMouseWheelUp()
	{
		m_Queue.push(MouseEvent(MouseEvent::MouseEventType::WheelUp, *this));
		TrimBuffer();
	}

	void Mouse::OnMouseWheelDown()
	{
		m_Queue.push(MouseEvent(MouseEvent::MouseEventType::WheelDown, *this));
		TrimBuffer();
	}

	void Mouse::OnMouseWheelDelta(int delta)
	{
		m_WheelDeltaCarry += delta;
		while (m_WheelDeltaCarry >= WHEEL_DELTA)
		{
			m_WheelDeltaCarry -= WHEEL_DELTA;
			OnMouseWheelUp();
		}

		while (m_WheelDeltaCarry <= -WHEEL_DELTA)
		{
			m_WheelDeltaCarry += WHEEL_DELTA;
			OnMouseWheelDown();
		}

	}

	void Mouse::OnMouseMove(int x, int y)
	{
		m_Pos += {x, y};
		m_Queue.push(MouseEvent(MouseEvent::MouseEventType::Move, *this));
		TrimBuffer();
	}

	void Mouse::TrimBuffer()
	{
		while (m_Queue.size() > m_nQueueSize)
		{
			m_Queue.pop();
		}
	}
}


