#include "Button.h"


namespace NPE
{
	Button::Button(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color, const std::string& text)
		: Control(renderer, Control::Type::Button, pos, size, color, text)
	{

	}
}


