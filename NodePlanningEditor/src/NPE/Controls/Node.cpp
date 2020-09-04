#include "Node.h"
#include "NPE/Graphics/Renderer2D.h"


namespace NPE
{
	//TODO: Measure speed and test if variables are being moved instead of coppied
	Node::Node(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color)
		: Control(renderer, Control::Type::Node, pos, size, color)
	{
		//m_Children.emplace_back(new Button(renderer, { pos.x + size.width - 30, pos.y + 15 }, { 10, 10 }, { 160.0f, 160.0f, 160.0f }, ""));
		m_Children.emplace_back(new Button(renderer, { pos.x + (size.width / 2) - (10 / 2), pos.y + 5 }, { 10, 10 }, { 160.0f, 160.0f, 160.0f }, ""));
	}
	
	bool Node::Render() const
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
}


