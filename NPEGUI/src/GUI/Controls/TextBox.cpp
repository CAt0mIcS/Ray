#include "pch.h"
#include "TextBox.h"

#include "GUI/Graphics/Renderer.h"
#include "GUI/Graphics/TextRenderer.h"

#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"

/**
* QUESTION:
*	Should I be designing something like this GUI library only for this project or should I make it usable in another one?
*	Should I write class declarations (class Node;) in header files instead of including them
*/


using namespace Util;

namespace GUI
{
	TextBox::TextBox(Control* parent)
		: Control(parent), m_Text(L""), m_FontFamily(L"Consolas"), m_FontSize(0), m_IsMultiline(false), m_TextBoxWindow(this, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300)
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
		return false;
	}

	//TODO: Check if the text is too long and implement scrolling here
	void TextBox::RenderText()
	{
		NText text;
		text.text = m_Text;
		text.fontFamily = m_FontFamily;
		
		float xOffset = m_Size.width / 30.0f;
		float yOffset;
		if (m_IsMultiline)
		{
			yOffset = m_Size.height / 10.0f;
		}
		else
		{
			yOffset = m_Size.height / 2.0f - m_FontSize / 2.0f;
		}

		text.fontSize = m_FontSize;

		text.pos = Util::NPoint{ m_Pos.x + xOffset, m_Pos.y + yOffset };
		text.size = { m_Size.width - xOffset, m_Size.height - yOffset };

		auto metrics = TextRenderer::Get().GetTextMetrics(text);


		TextRenderer::Get().RenderText(text);
	}

	std::optional<std::pair<Util::NPoint, Util::NSize>> TextBox::CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize)
	{
		return { {{}, {}} };
	}

	bool TextBox::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == MouseButton::Left)
		{
			if (m_IsMultiline)
			{
				//display edit window
				m_TextBoxWindow.Show();
			}
			else
			{
				//use normal single-line edit

			}
			return true;
		}
		return false;
	}

}

