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
		
		
		float spaceTxtboxXToNodeBorder = m_Size.width / 20;
		float x = m_Pos.x + spaceTxtboxXToNodeBorder;

		float spaceTxtboxYToLineButton = m_Size.width / 40;
		Control* lastLineButton = m_Children[m_Children.size() - 1];
		float y = lastLineButton->GetPos().y + lastLineButton->GetSize().height + spaceTxtboxYToLineButton;

		//20% of Node is textbox
		float width = m_Size.width - spaceTxtboxXToNodeBorder * 2;

		float heightPercentOfNode = 0.2f;
		float height = m_Size.height * heightPercentOfNode;

		m_Children.emplace_back(new TextBox(renderer, { x, y }, { width, height }, { 15.0f, 17.0f, 19.0f }, L"Hello World"));
		
		//new TextBox(m_Window.Renderer2D, { 10, 10 }, { 1000, 1000 }, { 255, 255, 255 }, L"Hello World")

	}
	
	bool Node::Render() const
	{
		if (this->IsInWindow())
		{
			m_Renderer.RenderRoundedRectControl(*this);

			for (auto& child : m_Children)
				child->Render();

			return true;
		}

		return false;
	}
}


