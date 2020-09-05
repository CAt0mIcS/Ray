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
		//5.177 percent of node x is button
		float widthPercentOfNode = 0.05177f;
		float btnWidth = m_Size.width * widthPercentOfNode;
		float spaceButtonXToNodeBorder = (m_Size.width / 2) - (btnWidth / 2);
		float btnX = m_Pos.x + spaceButtonXToNodeBorder;

		float yOffset = m_Size.height / 56;
		float btnY = m_Pos.y + yOffset;

		float widthAndHeight = std::min(m_Size.width, m_Size.height) / 12;

		m_Children.emplace_back(new Button(renderer, { btnX, btnY }, { widthAndHeight, widthAndHeight }, { 160.0f, 160.0f, 160.0f }, ""));
		
		float spaceTxtboxXToNodeBorder = m_Size.width / 20;
		float x = m_Pos.x + spaceTxtboxXToNodeBorder;

		float spaceTxtboxYToLineButton = m_Size.width / 40;
		Control* lastLineButton = m_Children[m_Children.size() - 1];
		float y = lastLineButton->GetPos().y + lastLineButton->GetSize().height + spaceTxtboxYToLineButton;

		//20% of Node is textbox
		float width = m_Size.width - spaceTxtboxXToNodeBorder * 2;

		float heightPercentOfNode = 0.2f;
		float height = m_Size.height * heightPercentOfNode;

		m_Children.emplace_back(new TextBox(renderer, { x, y }, { width, height }, { 15.0f, 17.0f, 19.0f }, L""));
		
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


