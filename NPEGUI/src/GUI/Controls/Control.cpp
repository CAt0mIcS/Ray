#include "pch.h"
#include "Control.h"

#include "GUI/Handlers/Mouse.h"
#include "GUI/Graphics/Renderer.h"

using namespace Util;


namespace GUI
{
	unsigned int Control::m_NextId = 0;

	Control* Control::GetEventReceiver(Event& e)
	{
		Control* receiver = nullptr;
		for (auto* child : GetChildren())
		{
			receiver = child->GetEventReceiver(e);
		}

		//no child control was clicked, check if mouse is on node control
		if (receiver == nullptr)
		{
			if (Mouse::IsOnControl(this))
				receiver = this;
		}

		return receiver;
	}

	Control::Control(Control* parent)
		: m_Parent(parent), m_Id(m_NextId), m_Pos{}, m_Size{}, m_Color{}, m_Type(Type::INVALID)
	{
		++m_NextId;
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
			NPoint ratio{ pos.x / m_Pos.x, pos.y / m_Pos.y };
			child->m_Pos += ratio;
		}

		m_Pos = pos;
	}

	void Control::ResizeBy(const NSize& size)
	{
		m_Size += size;
		
		for (auto& child : m_Children)
		{
			auto newLayout = child->CalculateLayout(m_Pos, m_Size);

			if (newLayout)
			{
				child->MoveTo(newLayout.value().first);
				child->ResizeTo(newLayout.value().second);
			}
		}

	}

	void Control::ResizeTo(const NSize& size)
	{
		m_Size = size;

		for (auto& child : m_Children)
		{
			auto newLayout = child->CalculateLayout(m_Pos, m_Size);
			
			if (newLayout)
			{
				child->MoveTo(newLayout.value().first);
				child->ResizeTo(newLayout.value().second);
			}
		}
	}

	//TODO: Implement function
	bool Control::OverlapsWith(const Control* other, const NSize& minDst)
	{
		//return m_Renderer.RoundedRectConrolsOverlap(*this, *other, minDst);
		return false;
	}

	bool Control::IsInWindow() const
	{
		//RECT rc;
		//GetWindowRect(Renderer::Get().GetNativeWindow(), &rc);

		//return m_Pos.x + m_Size.width > 0 && m_Pos.y + m_Size.height > 0 && m_Pos.x < rc.right && m_Pos.y < rc.bottom;
		return true;
	}

}

