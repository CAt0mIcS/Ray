#include "pch.h"
#include "NodeRect.h"

#include "NPE/Window/MainWindow.h"
#include <gdiplus.h>

#define DIV 2.25f
namespace wrl = Microsoft::WRL;

namespace NPE
{

	int NodeRect::m_NextId = 0;

	NodeRect::NodeRect(MainWindow* parent, const NPoint& pos, const NSize& size)
		: m_Id(m_NextId), m_Pos(pos), m_Size(size)
	{

		Gdiplus::Rect rc(m_Pos.x, m_Pos.y, m_Size.width, m_Size.height);
		m_RoundedRect = { (float)rc.GetLeft(), (float)rc.GetTop(), (float)rc.GetRight(), (float)rc.GetBottom(), 30, 30 };
		++m_NextId;
	}

	void NodeRect::MoveBy(const NPoint& pos)
	{
		m_Pos += pos;
		Gdiplus::Rect rc(m_Pos.x / DIV, m_Pos.y / DIV, m_Size.width, m_Size.height);

		m_RoundedRect = { (float)rc.GetLeft(), (float)rc.GetTop(), (float)rc.GetRight(), (float)rc.GetBottom(), 30, 30 };
	}

	void NodeRect::MoveTo(const NPoint& pos)
	{
		m_Pos = pos;
		Gdiplus::Rect rc(m_Pos.x / DIV, m_Pos.y / DIV, m_Size.width, m_Size.height);

		m_RoundedRect = { (float)rc.GetLeft(), (float)rc.GetTop(), (float)rc.GetRight(), (float)rc.GetBottom(), 30, 30 };
	}

	void NodeRect::ResizeBy(const NSize& size)
	{
		m_Size += size;
		Gdiplus::Rect rc(m_Pos.x, m_Pos.y, m_Size.width / DIV, m_Size.height / DIV);
	
		m_RoundedRect = { (float)rc.GetLeft(), (float)rc.GetTop(), (float)rc.GetRight(), (float)rc.GetBottom(), 30, 30 };
	}

	void NodeRect::ResizeTo(const NSize& size)
	{
		m_Size = size;
		Gdiplus::Rect rc(m_Pos.x, m_Pos.y, m_Size.width / DIV, m_Size.height / DIV);

		m_RoundedRect = { (float)rc.GetLeft(), (float)rc.GetTop(), (float)rc.GetRight(), (float)rc.GetBottom(), 30, 30 };
	}
}


