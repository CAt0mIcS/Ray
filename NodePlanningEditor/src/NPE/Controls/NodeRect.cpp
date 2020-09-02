#include "pch.h"
#include "NodeRect.h"

#include "NPE/Window/MainWindow.h"


namespace wrl = Microsoft::WRL;

namespace NPE
{

	int NodeRect::m_NextId = 0;

	NodeRect::NodeRect(MainWindow* parent, const NPoint& pos, const NSize& size)
		: m_Id(m_NextId), m_RoundedRect{ { pos.x, pos.y, pos.x + size.width, pos.y + size.height }, 30, 30 }
	{
		++m_NextId;
	}

	void NodeRect::MoveBy(const NPoint& pos)
	{
		//m_Pos += pos;
		//m_RoundedRect = { { m_Pos.x, m_Pos.y, m_Pos.x + m_Size.width, m_Pos.y + m_Size.height }, 30, 30 };
		m_RoundedRect = { {m_RoundedRect.rect.left + pos.x, m_RoundedRect.rect.top + pos.y, m_RoundedRect.rect.right, m_RoundedRect.rect.bottom}, 30, 30 };
	}

	void NodeRect::MoveTo(const NPoint& pos)
	{
		//m_Pos = pos;
		//m_RoundedRect = { { m_Pos.x, m_Pos.y, m_Pos.x + m_Size.width, m_Pos.y + m_Size.height }, 30, 30 };
		m_RoundedRect = { {pos.x, pos.y, m_RoundedRect.rect.right, m_RoundedRect.rect.bottom}, 30, 30 };
	}

	void NodeRect::ResizeBy(const NSize& size)
	{
		//m_Size += size;
		//m_RoundedRect = { { m_Pos.x, m_Pos.y, m_Pos.x + m_Size.width, m_Pos.y + m_Size.height }, 30, 30 };
	
		m_RoundedRect.rect.right = m_RoundedRect.rect.left + (m_RoundedRect.rect.right - m_RoundedRect.rect.left) + size.width;
		m_RoundedRect.rect.bottom = m_RoundedRect.rect.bottom + (m_RoundedRect.rect.bottom - m_RoundedRect.rect.top) + size.height;
	}

	void NodeRect::ResizeTo(const NSize& size)
	{
		//m_Size = size;
		//m_RoundedRect = { { m_Pos.x, m_Pos.y, m_Pos.x + m_Size.width, m_Pos.y + m_Size.height }, 30, 30 };

		m_RoundedRect.rect.right = (m_RoundedRect.rect.right - m_RoundedRect.rect.left) + size.width;
		m_RoundedRect.rect.bottom = (m_RoundedRect.rect.bottom - m_RoundedRect.rect.top) + size.height;
	}
}


