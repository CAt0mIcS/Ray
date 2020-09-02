#include "pch.h"
#include "NodeEllipse.h"

#include "NPE/Window/MainWindow.h"


namespace NPE
{

	MainWindow* parentWin;
	int NodeEllipse::m_NextId = 0;

	NodeEllipse::NodeEllipse(MainWindow* parent, const NPoint& pos, const NSize& size)
		: m_Pos(pos), m_Size(size), m_Id(m_NextId)
	{
		++m_NextId;
		parentWin = parent;
	}

	void NodeEllipse::MoveBy(const NPoint& pos)
	{
		m_Pos += pos;
		RECT rc{ m_Pos.x - 25, m_Pos.y - 25, m_Pos.x + m_Size.width + 25, m_Pos.y + m_Size.height + 25 };
		InvalidateRect(parentWin->GetNativeWindow(), &rc, TRUE);
		UpdateWindow(parentWin->GetNativeWindow());
	}

	void NodeEllipse::MoveTo(const NPoint& pos)
	{
		m_Pos = pos;
		RECT rc{ m_Pos.x - 25, m_Pos.y - 25, m_Pos.x + m_Size.width + 25, m_Pos.y + m_Size.height + 25 };
		InvalidateRect(parentWin->GetNativeWindow(), &rc, TRUE);
		UpdateWindow(parentWin->GetNativeWindow());
	}

	void NodeEllipse::ResizeBy(const NSize& size)
	{
		m_Size += size;
		RECT rc{ m_Pos.x - 25, m_Pos.y - 25, m_Pos.x + m_Size.width + 25, m_Pos.y + m_Size.height + 25 };
		InvalidateRect(parentWin->GetNativeWindow(), &rc, TRUE);
		UpdateWindow(parentWin->GetNativeWindow());
	}

	void NodeEllipse::ResizeTo(const NSize& size)
	{
		m_Size = size;
		RECT rc{ m_Pos.x - 25, m_Pos.y - 25, m_Pos.x + m_Size.width + 25, m_Pos.y + m_Size.height + 25 };
		InvalidateRect(parentWin->GetNativeWindow(), &rc, TRUE);
		UpdateWindow(parentWin->GetNativeWindow());
	}

	void NodeEllipse::Draw(HDC hDC)
	{
		if (!RoundRect(hDC, m_Pos.x, m_Pos.y, m_Pos.x + m_Size.width, m_Pos.y + m_Size.height, 100, 100))
			throw std::exception("Failed to draw ellipse");
	}
}


