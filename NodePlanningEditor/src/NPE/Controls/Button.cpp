#include "Button.h"
#include "NPE/Graphics/Renderer2D.h"


namespace NPE
{
	std::function<void(Button&)> Button::m_OnButtonClickedCallback;

	Button::Button(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color, const std::string& text)
		: Control(renderer, Control::Type::Button, pos, size, color), m_Text(text)
	{

	}

	bool Button::Render() const
	{
		if (this->IsInWindow())
		{
			m_Renderer.RenderRoundedRectControl(*this);

			for (auto& child : m_Children)
				m_Renderer.RenderRoundedRectControl(*child);
			
			return true;
		}
		return false;
	}
	
	std::optional<std::pair<NPoint, NSize>> Button::CalculateLayout(const NPoint& parentPos, const NSize& parentSize)
	{
		float middleOfNodeX = parentPos.x + (parentSize.width / 2);

		float btnWidthHeight = std::min(parentSize.width, parentSize.height) / 12;
		float btnX = middleOfNodeX - (btnWidthHeight / 2);

		float yOffsetBtn = parentSize.height / 56;
		float btnY = parentPos.y + yOffsetBtn;

		return { { { btnX, btnY }, { btnWidthHeight, btnWidthHeight } } };
	}
}


