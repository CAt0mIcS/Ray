#include "pch.h"
#include "Control.h"

#include "NPE/Window/MainWindow.h"


namespace NPE
{
	Control::Control(MainWindow* parent, const Type type, const NPoint& pos, const NSize& size)
		: m_hWndParent(parent->GetNativeWindow()), m_Type(type), m_Id(m_NextId), m_Pos(pos), m_Size(size), m_hWnd(0)
	{
		++m_NextId;
	}

	void Control::MoveBy(const NPoint& pos)
	{
		m_Pos += pos;
		if (!MoveWindow(m_hWnd, m_Pos.x, m_Pos.y, m_Size.width, m_Size.height, TRUE))
			return;
	}

	void Control::MoveTo(const NPoint& pos)
	{
		m_Pos = pos;
		if (!MoveWindow(m_hWnd, m_Pos.x, m_Pos.y, m_Size.width, m_Size.height, TRUE))
			return;
	}

	void Control::ResizeBy(const NSize& size)
	{
		m_Size += size;
		if (!MoveWindow(m_hWnd, m_Pos.x, m_Pos.y, m_Size.width, m_Size.height, TRUE))
			return;
	}

	void Control::ResizeTo(const NSize& size)
	{
		m_Size = size;
		if (!MoveWindow(m_hWnd, m_Pos.x, m_Pos.y, m_Size.width, m_Size.height, TRUE))
			return;
	}

	bool Control::IsInWindow() const
	{
		RECT rc;
		GetWindowRect(m_hWndParent, &rc);
		return m_Pos.x + m_Size.width > 0 && m_Pos.y + m_Size.height > 0 && m_Pos.x < rc.right && m_Pos.y < rc.bottom;
	}

}

