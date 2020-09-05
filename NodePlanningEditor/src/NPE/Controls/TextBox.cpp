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

			float xOffset = m_Size.width / 81.0f;
			float yOffset = m_Size.height / -11.2f;
			m_Renderer.RenderText(m_Text, { m_Pos.x + xOffset, m_Pos.y + yOffset }, { 160.0f, 160.0f, 160.0f }, fontSize);
			return true;
		}

		return false;
	}
	
	std::optional<std::pair<NPoint, NSize>> TextBox::CalculateLayout(const NPoint& parentPos, const NSize& parentSize)
	{
		float xOffsetTxt = parentSize.width / 22.5f;
		float txtX = parentPos.x + xOffsetTxt;

		float yOffsetTxt = parentSize.height / 7.0f;
		float txtY = parentPos.y + yOffsetTxt;

		float txtWidth = parentSize.width - (xOffsetTxt * 2);

		//20% of Node's height is this textbox
		float txtPercentOfNode = 0.2f;
		float txtHeight = parentSize.height * txtPercentOfNode;

		return { { {txtX, txtY}, {txtWidth, txtHeight} } };
	}
}


