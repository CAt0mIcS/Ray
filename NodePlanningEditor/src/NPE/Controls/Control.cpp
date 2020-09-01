#include "pch.h"
#include "Control.h"


namespace NPE
{

	Control::Control(const Type type, const NPoint& pos, const NSize& size)
		: m_Type(type), m_Id(m_NextId), m_Pos(pos), m_Size(size), m_hWnd(0)
	{
		++m_NextId;
	}

	void Control::MoveBy(const NPoint& pos)
	{
		m_Pos += pos;
		MoveWindow(m_hWnd, m_Pos.x, m_Pos.y, m_Size.width, m_Size.height, TRUE);
	}

	void Control::MoveTo(const NPoint& pos)
	{
		m_Pos = pos;
		MoveWindow(m_hWnd, m_Pos.x, m_Pos.y, m_Size.width, m_Size.height, TRUE);
	}

}

