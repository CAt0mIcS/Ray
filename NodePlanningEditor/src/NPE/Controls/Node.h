#pragma once

#include "Control.h"
#include "Button.h"


namespace NPE
{
	class Node : public Control
	{
	public:
		Node(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color, const std::string& text);

	private:
	};
}



