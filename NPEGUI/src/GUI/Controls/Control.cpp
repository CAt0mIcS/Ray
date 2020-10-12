#include "pch.h"
#include "Control.h"

#include "GUI/Handlers/Mouse.h"
#include "GUI/Graphics/Renderer.h"

#include "GUI/Events/MouseEvent.h"


namespace GUI
{
	unsigned int Control::s_NextId = 0;
	Control* Control::s_Focus = nullptr;

	Control* Control::GetEventReceiver(_In_ Event& e)
	{
		Control* receiver = nullptr;
		for (auto* child : GetChildren())
		{
			receiver = child->GetEventReceiver(e);
			if (receiver)
				break;
		}

		//no child Control was clicked, check if mouse is on Node Control
		if (receiver == nullptr)
		{
			if (Mouse::IsOnControl(this))
				receiver = this;
		}

		return receiver;
	}

	void Control::PostRedraw() const
	{
		RECT rc = Util::ToRect(m_Pos, m_Size);

		InvalidateRect(Renderer::Get().GetNativeWindow(), &rc, TRUE);
		UpdateWindow(Renderer::Get().GetNativeWindow());
	}

	Control::Control(_In_ Type type, _In_opt_ Control* parent)
		: m_Parent(parent), m_Id(s_NextId), m_Pos{}, m_Size{}, m_Color{}, m_Type(type)
	{
		++s_NextId;
	}

	void Control::MoveBy(_In_ const Util::NPoint& pos)
	{
		for (auto& child : m_Children)
		{
			child->m_Pos += pos;
		}

		m_Pos += pos;
	}

	void Control::MoveTo(_In_ const Util::NPoint& pos)
	{
		for (auto& child : m_Children)
		{
			Util::NPoint ratio{ pos.x / m_Pos.x, pos.y / m_Pos.y };
			child->m_Pos += ratio;
		}

		m_Pos = pos;
	}

	void Control::ResizeBy(_In_ const Util::NSize& size)
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

	void Control::ResizeTo(_In_ const Util::NSize& size)
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
	bool Control::OverlapsWith(_In_ const Control* other, _In_opt_ const Util::NSize& minDst)
	{
		//return m_Renderer.RoundedRectConrolsOverlap(*this, *other, minDst);
		return false;
	}

	bool Control::IsInWindow() const
	{

		/// <TODO>
		/// Perfect function, parts are still rendered outside of window rect
		/// </TODO>

		RECT rc;
		GetWindowRect(Renderer::Get().GetNativeWindow(), &rc);
		
		auto pos = m_Pos;
		auto matrix = Renderer::Get().GetViewMatrix();

		pos.x = (m_Pos.x * matrix.m11 + m_Pos.y * matrix.m21 + matrix.dx);
		pos.y = (m_Pos.x * matrix.m12 + m_Pos.y * matrix.m22 + matrix.dy);

		bool result = pos.x + m_Size.width > 0 && pos.y + m_Size.height > 0 && pos.x < rc.right && pos.y < rc.bottom;
		return result;
	}

	Control::~Control()
	{
		if (this->HasFocus())
		{
			s_Focus = nullptr;
		}

		for (auto* child : m_Children)
		{
			delete child;
		}
	}

}

