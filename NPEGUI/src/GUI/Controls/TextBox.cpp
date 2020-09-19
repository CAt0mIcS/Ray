#include "pch.h"
#include "TextBox.h"

#include "GUI/Graphics/Renderer.h"
#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"
#include "GUI/Events/ApplicationEvent.h"

#include "Util/Exceptions.h"
#include "Util/Direct2D.h"

/**
* QUESTION:
*	Should I be designing something like this GUI library only for this project or should I make it usable in another one?
*	Should I write class declarations (class Node;) in header files instead of including them
*/



namespace GUI
{
	TextBox::TextBox(Control* parent)
		: Control(parent), m_Text{}, m_IsMultiline(false), m_Caret(this)
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
			m_Caret.Render();
			RenderText();

			return true;
		}

		return false;
	}

	bool TextBox::OnEvent(Event& e)
	{

		switch (e.GetType())
		{
		case EventType::MouseButtonPressedEvent:
		{
			m_Caret.OnMouseButtonPressed((MouseButtonPressedEvent&)e);
			return false;
		}
		case EventType::MouseButtonReleasedEvent:
		{
			m_Caret.OnMouseButtonReleased((MouseButtonReleasedEvent&)e);
			return false;
		}
		case EventType::KeyPressedEvent:
		{
			m_Caret.OnKeyPressed((KeyPressedEvent&)e);
			return false;
		}
		case EventType::MouseMoveEvent:
		{
			m_Caret.OnMouseMove((MouseMoveEvent&)e);
			return false;
		}
		case EventType::CharEvent:
		{
			m_Caret.OnCharEvent((CharEvent&)e);
			return false;
		}
		case EventType::SetCursorEvent:
		{
			/**
			* Mouse is already guaranteed to be on the control, we don't need to check here
			* Default cursor is automatically restored when exiting the control
			*/
			SetCursor(LoadCursor(NULL, IDC_IBEAM));
			return true;
		}
		}

		if (e.GetType() == EventType::MouseButtonPressedEvent)
		{
		}
		else if (e.GetType() == EventType::KeyPressedEvent)
		{
		}
		else if (e.GetType() == EventType::MouseMoveEvent)
		{
		}
		else if (e.GetType() == EventType::CharEvent)
		{
		}
		else if (e.GetType() == EventType::SetCursorEvent)
		{
			
		}
		
		return false;
	}

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

}


