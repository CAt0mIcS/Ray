#include "rlpch.h"
#include "Mouse.h"

#include "Reyal/Widgets/ZWidget.h"


namespace Zeal::Reyal
{
	Mouse::Mouse()
		: m_IsLeftPressed(false), m_IsMiddlePressed(false), m_IsRightPressed(false), m_MousePos{}
	{
	}

	bool Mouse::IsLeftPressed() const
	{
		return m_IsLeftPressed;
	}
	
	bool Mouse::IsMiddlePressed() const
	{
		return m_IsMiddlePressed;
	}
	
	bool Mouse::IsRightPressed() const
	{
		return m_IsRightPressed;
	}

	bool Mouse::IsOnWidget(const Widget* widget) const
	{
		return false;
	}

	const Util::Point& Mouse::GetMousePos() const
	{
		return m_MousePos;
	}

	void Mouse::SetMousePos(const Util::Point& mousePos)
	{
		m_MousePos = mousePos;
	}
}


