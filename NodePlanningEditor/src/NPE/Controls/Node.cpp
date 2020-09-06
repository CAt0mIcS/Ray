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
		float middleOfNodeX = m_Pos.x + (m_Size.width / 2);

		float btnHeightAndHeight = std::min(m_Size.width, m_Size.height) / 12;
		float btnX = middleOfNodeX - (btnHeightAndHeight / 2);
		
		float yOffsetBtn = m_Size.height / 56;
		float btnY = m_Pos.y + yOffsetBtn;

		m_Children.emplace_back(new Button(renderer, { btnX, btnY }, { btnHeightAndHeight, btnHeightAndHeight }, { 160.0f, 160.0f, 160.0f }, ""));
		

		float xOffsetTxt = m_Size.width / 22.5f;
		float txtX = m_Pos.x + xOffsetTxt;

		float yOffsetTxt = m_Size.height / 7.0f;
		float txtY = m_Pos.y + yOffsetTxt;
		
		float txtWidth = m_Size.width - (xOffsetTxt * 2);

		//20% of Node's height is this textbox
		float txtPercentOfNode = 0.2f;
		float txtHeight = m_Size.height * txtPercentOfNode;

		m_Children.emplace_back(new TextBox(renderer, { txtX, txtY }, { txtWidth, txtHeight }, { 15.0f, 17.0f, 19.0f }, L"MainWindow"));
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


