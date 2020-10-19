#include "rlpch.h"
#include "Mouse.h"


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
	
	const Util::ZPoint& Mouse::GetMousePos() const
	{
		return m_MousePos;
	}

	void Mouse::SetMousePos(const Util::ZPoint& mousePos)
	{
		m_MousePos = mousePos;
	}
}


