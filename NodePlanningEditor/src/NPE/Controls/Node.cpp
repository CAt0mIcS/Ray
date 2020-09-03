#include "Node.h"


namespace NPE
{
	//TODO: Measure speed and test if variables are being moved instead of coppied
	Node::Node(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color, const std::string& text)
		: Control(renderer, Control::Type::Node, pos, size, color, text)
	{
		m_Children.emplace_back(Button(renderer, { pos.x + size.width - 30, pos.y + 15 }, { 10, 10 }, { 160.0f, 160.0f, 160.0f }, ""));
	}
}


