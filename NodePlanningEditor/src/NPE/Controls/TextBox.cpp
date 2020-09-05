#include "TextBox.h"

#include "NPE/Graphics/Renderer2D.h"


namespace NPE
{
	std::function<void(TextBox&)> TextBox::m_OnTextBoxClickedCallback;

	TextBox::TextBox(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color, const std::wstring& startText)
		: Control(renderer, Type::TextBox, pos, size, color), m_Text(startText)
	{
	}

	bool TextBox::Render() const
	{
		if (this->IsInWindow())
		{
			const float radius = std::max(m_Size.width, m_Size.height) / 18;
			
			m_Renderer.RenderRoundedRectControl(*this, radius, radius);
			m_Renderer.RenderRoundedRectBorder(m_Pos, m_Size, { 44, 62, 80 }, radius, radius);
			RenderText();
			return true;
		}

		return false;
	}

	bool TextBox::RenderText() const
	{
		if (this->IsInWindow())
		{
			const float fontSize = std::max(m_Size.width, m_Size.height) / 7.2f;

			//405, 56
			float xOffset = m_Size.width / 81.0f;
			float yOffset = m_Size.height / -11.2f;
			m_Renderer.RenderText(m_Text, { m_Pos.x + xOffset, m_Pos.y + yOffset }, { 160.0f, 160.0f, 160.0f }, fontSize);
			return true;
		}

		return false;
	}
}


