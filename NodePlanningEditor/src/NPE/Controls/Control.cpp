#include "pch.h"
#include "Control.h"

#include "NPE/Graphics/Renderer2D.h"


namespace NPE
{
	Control::Control(Renderer2D& renderer, const Type type, const NPoint& pos, const NSize& size, const NColor& color, const std::string& text)
		: m_Renderer(renderer), m_Type(type), m_Pos(pos), m_Size(size), m_Text(text), m_Color(color), m_Children{}
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

	//TODO: fix return type
	bool Control::Render()
	{
		m_Renderer.RenderRoundedRectControl(*this);
		
		for (auto& child : m_Children)
			m_Renderer.RenderRoundedRectControl(child);

		return false;
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

