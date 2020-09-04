#include "pch.h"
#include "Control.h"

#include "NPE/Graphics/Renderer2D.h"


namespace NPE
{
	Control::Control(Renderer2D& renderer, const Type type, const NPoint& pos, const NSize& size, const NColor& color)
		: m_Renderer(renderer), m_Type(type), m_Pos(pos), m_Size(size), m_Children{}, m_Color(color)
	{
	}

	void Control::MoveBy(const NPoint& pos)
	{
		for (auto& child : m_Children)
		{
			child->m_Pos += pos;
		}

		m_Pos += pos;
	}

	void Control::MoveTo(const NPoint& pos)
	{
		for (auto& child : m_Children)
		{
			child->m_Pos = pos;
		}

		m_Pos = pos;
	}

	void Control::ResizeBy(const NSize& size)
	{
		for (auto& child : m_Children)
		{
			child->m_Size += size;
		}

		m_Size += size;
	}

	void Control::ResizeTo(const NSize& size)
	{
		for (auto& child : m_Children)
		{
			NSize ratio = size / m_Size;
			child->m_Size *= ratio;
		}

		m_Size = size;
	}

	//TODO: fix function
	bool Control::IsInWindow() const
	{
		//RECT rc;
		//GetWindowRect(m_hWndParent, &rc);
		//return m_Pos.x + m_Size.width > 0 && m_Pos.y + m_Size.height > 0 && m_Pos.x < rc.right && m_Pos.y < rc.bottom;
		return false;
	}

}

