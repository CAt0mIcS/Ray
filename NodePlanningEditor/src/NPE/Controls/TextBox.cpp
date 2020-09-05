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
			m_Renderer.RenderRoundedRectControl(*this);
			RenderText();
			return true;
		}

		return false;
	}

	bool TextBox::RenderText() const
	{
		if (this->IsInWindow())
		{
			m_Renderer.RenderText(m_Text, { 100, 100 }, { 100, 100 });
			return true;
		}

		return false;
	}
}


