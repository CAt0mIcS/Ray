#pragma once

#include "Control.h"


namespace NPE
{
	class Button : public Control
	{
	public:
		Button(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color, const std::string& text);

	private:
	};
}

