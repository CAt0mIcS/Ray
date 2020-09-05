#include "Node.h"
#include "NPE/Graphics/Renderer2D.h"

#include "Button.h"
#include "TextBox.h"


namespace NPE
{
	std::function<void(Node&)> Node::m_OnNodeClickedCallback;

	//TODO: Measure speed and test if variables are being moved instead of coppied
	Node::Node(Renderer2D& renderer, const NPoint& pos, const NSize& size, const NColor& color)
		: Control(renderer, Control::Type::Node, pos, size, color)
	{
		float minWiHi = std::min(m_Size.width, m_Size.height);
		m_Children.emplace_back(new Button(renderer, { m_Pos.x + (m_Size.width / 2) - ((m_Size.width / 20) / 2), m_Pos.y + 5 }, { minWiHi / 12, minWiHi / 12 }, { 160.0f, 160.0f, 160.0f }, ""));
		m_Children.emplace_back(new TextBox(renderer, {m_Pos.x, m_Pos.y}, {minWiHi / 5, minWiHi / 5}, { 120.0f, 120.0f, 120.0f }));
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


