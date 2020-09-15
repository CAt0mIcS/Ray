#include "pch.h"
#include "TextBox.h"

#include "GUI/Graphics/Renderer.h"
#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"
#include "GUI/Events/ApplicationEvent.h"

#include "Util/Debug/Timer.h"

/**
* QUESTION:
*	Should I be designing something like this GUI library only for this project or should I make it usable in another one?
*	Should I write class declarations (class Node;) in header files instead of including them
*/



namespace GUI
{
	TextBox::TextBox(Control* parent)
		: Control(parent), m_Text{}, m_CurrentlySelecting(true), m_IsMultiline(false)
	{
		
	}

	bool TextBox::Render()
	{
		if (this->IsInWindow())
		{
			for (auto* child : m_Children)
			{
				child->Render();
			}

			const float max = std::min(GetSize().width, GetSize().height);
			Renderer::Get().RenderRoundedRect(GetPos(), GetSize(), GetColor(), max / 5.0f, max / 5.0f);
			RenderText();

			return true;
		}

		return false;
	}

	bool TextBox::OnEvent(Event& e)
	{
		if (e.GetType() == EventType::MouseButtonPressedEvent)
		{
			return OnMouseButtonPressed((MouseButtonPressedEvent&)e);
		}
		else if (e.GetType() == EventType::MouseButtonReleasedEvent)
		{
			return OnMouseButtonReleased((MouseButtonReleasedEvent&)e);
		}
		else if (e.GetType() == EventType::SetCursorEvent)
		{
			return OnSetCursor((SetCursorEvent&)e);
		}
		
		return false;
	}

	//TODO: Check if the text is too long and implement scrolling here
	void TextBox::RenderText()
	{
		float xOffset = m_Size.width / 30.0f;
		float yOffset;
		if (m_IsMultiline)
		{
			yOffset = m_Size.height / 10.0f;
		}
		else
		{
			yOffset = m_Size.height / 2.0f - m_Text.fontSize / 2.0f;
		}

		m_Text.pos = Util::NPoint{ m_Pos.x + xOffset, m_Pos.y + yOffset };
		m_Text.size = { m_Size.width - xOffset, m_Size.height - yOffset };

		TextRenderer::Get().RenderText(m_Text);
	}

	std::optional<std::pair<Util::NPoint, Util::NSize>> TextBox::CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize)
	{
		return { {{}, {}} };
	}

	bool TextBox::OnSetCursor(SetCursorEvent& e)
	{
		if (Mouse::IsOnControl(this))
		{
			SetCursor(LoadCursor(NULL, IDC_IBEAM));
			return true;
		}
		else
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			return true;
		}
		return false;
	}

	bool TextBox::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == MouseButton::Left)
		{
			TIMER;
			m_CurrentlySelecting = true;
			
			BOOL isTrailingHit;
			BOOL isInside;
			
			auto metrics = TextRenderer::Get().HitTestPoint(m_Text, &isTrailingHit, &isInside);

 			return true;
		}
		return false;
	}

	bool TextBox::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (e.GetButton() == MouseButton::Left)
		{
			m_CurrentlySelecting = false;
			return true;
		}
		return false;
	}
}


