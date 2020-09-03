#include "pch.h"
#include "Control.h"


namespace NPE
{
	Control::Control(HWND parent, const Type type, const NPoint& pos, const NSize& size, const NColor& color, const std::string& text)
		: m_hWndParent(parent), m_Type(type), m_Pos(pos), m_Size(size), m_Text(text), m_Color(color)
	{
	}

	void Control::MoveBy(const NPoint& pos)
	{
		m_Pos += pos;
	}

	void Control::MoveTo(const NPoint& pos)
	{
		m_Pos = pos;
	}

	void Control::ResizeBy(const NSize& size)
	{
		m_Size += size;
	}

	void Control::ResizeTo(const NSize& size)
	{
		m_Size = size;
	}

	//TODO: fix function
	bool Control::IsInWindow() const
	{
		RECT rc;
		GetWindowRect(m_hWndParent, &rc);
		return m_Pos.x + m_Size.width > 0 && m_Pos.y + m_Size.height > 0 && m_Pos.x < rc.right && m_Pos.y < rc.bottom;
	}

}

