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
		if (e.GetType() == EventType::MouseButtonPressedEvent)
		{
			OnMouseButtonPressed((MouseButtonPressedEvent&)e);
		}
		else if (e.GetType() == EventType::MouseButtonReleasedEvent)
		{
			OnMouseButtonReleased((MouseButtonReleasedEvent&)e);
		}
		else if (e.GetType() == EventType::KeyPressedEvent)
		{
			OnKeyPressed((KeyPressedEvent&)e);
		}
		else if (e.GetType() == EventType::CharEvent)
		{
			OnCharEvent((CharEvent&)e);
		}
		else if (e.GetType() == EventType::SetCursorEvent)
		{
			/**
			* Mouse is already guaranteed to be on the control, we don't need to check here
			* Default cursor is automatically restored when exiting the control
			*/
			SetCursor(LoadCursor(NULL, IDC_IBEAM));
			return true;
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

	void TextBox::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		m_Caret.OnMouseButtonPressed(e);
	}

	void TextBox::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
	}

	void TextBox::OnKeyPressed(KeyPressedEvent& e)
	{
		m_Caret.OnKeyPressed(e);
	}

	void TextBox::OnCharEvent(CharEvent& e)
	{
		m_Caret.OnCharEvent(e);
	}
}


