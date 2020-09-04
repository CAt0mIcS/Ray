#include "Button.h"
#include "NPE/Graphics/Renderer2D.h"


namespace NPE
{
	std::function<void(Button&)> Button::m_OnButtonClickedCallback;

	Button::Button(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color, const std::string& text)
		: Control(renderer, Control::Type::Button, pos, size, color), m_Text(text)
	{

	}

	//TODO: fix return type
	bool Button::Render() const
	{
		m_Renderer.RenderRoundedRectControl(*this);

		for (auto& child : m_Children)
			m_Renderer.RenderRoundedRectControl(*child);

		return false;
	}
}


