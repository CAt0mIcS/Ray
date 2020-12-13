#include "Ray/rpch.h"
#include "Ray/Input/RMouse.h"

#include "Ray/Widgets/RWidget.h"

#include <RayDebug/RLogger.h>

namespace At0::Ray
{
	MouseInput::MouseInput()
		: m_IsLeftPressed(false), m_IsMiddlePressed(false), m_IsRightPressed(false), m_MousePos{}, m_OldMousePos{}
	{
	}

	MouseInput::~MouseInput()
	{
		RAY_LOG_DEBUG("[MouseInput] Destroyed");
	}

	bool MouseInput::IsLeftPressed() const
	{
		return m_IsLeftPressed;
	}
	
	bool MouseInput::IsMiddlePressed() const
	{
		return m_IsMiddlePressed;
	}
	
	bool MouseInput::IsRightPressed() const
	{
		return m_IsRightPressed;
	}

	bool MouseInput::IsOnWidget(const Widget* widget) const
	{
		return false;
	}

	bool MouseInput::IsOnWidget(const Scope<Widget>& widget) const
	{
		return IsOnWidget(widget.get());
	}

	const Point2& MouseInput::GetPos() const
	{
		return m_MousePos;
	}

	const Point2& MouseInput::GetOldPos() const
	{
		return m_OldMousePos;
	}

	void MouseInput::SetPos(const Point2& mousePos)
	{
		m_OldMousePos = m_MousePos;
		m_MousePos = mousePos;
	}
}


