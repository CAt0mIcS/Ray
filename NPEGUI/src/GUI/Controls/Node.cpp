#include "pch.h"
#include "Node.h"

#include "GUI/Graphics/Renderer.h"

#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"

#include "TextBox.h"
#include "Button.h"


namespace GUI
{
	Node::Node(_In_opt_ Control* parent)
		: Control(Type::Node, parent)
	{
	}

	void Node::Init()
	{
		TextBox* txtbox = (TextBox*)m_Children.emplace_back(new TextBox(this));
		float xOffsetTxt = m_Size.width / 22.5f;
		float txtX = m_Pos.x + xOffsetTxt;

		float yOffsetTxt = m_Size.height / 7.0f;
		float txtY = m_Pos.y + yOffsetTxt;

		float txtWidth = m_Size.width - (xOffsetTxt * 2);

		//20% of Node's height is this TextBox
		float txtPercentOfNode = 0.2f;
		float txtHeight = m_Size.height * txtPercentOfNode;

		txtbox->SetColor(Constants::g_DefaultTextBoxColor);
		txtbox->SetSize({ txtWidth, txtHeight });
		txtbox->SetPos({ txtX, txtY });
		txtbox->SetFontSize((m_Size.width + m_Size.height) / 24.3f);


		txtbox = (TextBox*)m_Children.emplace_back(new TextBox(this));
		yOffsetTxt = m_Size.height / 7.0f;
		txtY = m_Pos.y + yOffsetTxt + (m_Size.height / 3.5f);
		
		//40% of Node's height is this TextBox
		txtPercentOfNode = 0.4f;
		txtHeight = m_Size.height * txtPercentOfNode;
		
		txtbox->SetColor(Constants::g_DefaultTextBoxColor);
		txtbox->SetSize({ txtWidth, txtHeight });
		txtbox->SetPos({ txtX, txtY });
		txtbox->SetFontSize((m_Size.width + m_Size.height) / 48.6f);
		txtbox->SetMultiline(true);


		Button* btn = (Button*)m_Children.emplace_back(new Button(this));
		btn->SetColor(Constants::g_DefaultLineButtonColor);
	
		float middleOfNodeX = m_Pos.x + (m_Size.width / 2);

		float btnWidthAndHeight = std::min(m_Size.width, m_Size.height) / 12;
		float btnX = middleOfNodeX - (btnWidthAndHeight / 2);

		float yOffsetBtn = m_Size.height / 56;
		float btnY = m_Pos.y + yOffsetBtn;

		btn->SetPos({ btnX, btnY });
		btn->SetSize({ btnWidthAndHeight, btnWidthAndHeight });
		
	}

	bool Node::Render()
	{
		if (this->IsInWindow())
		{
			const float max = std::max(GetSize().width, GetSize().height);
			Renderer::Get().RenderRoundedRect(GetPos(), GetSize(), GetColor(), max / 5.0f, max / 5.0f);

			constexpr float minWidth = 48.2f, minHeight = 30.0f;
			for (auto* child : m_Children)
			{
				if (m_Size.width > minWidth && m_Size.height > minHeight)
				{
					child->Render();
				}
				else if (m_Size.width <= minWidth && m_Size.height <= minHeight && child->GetType() == Type::Button)
				{
					child->Render();
				}
			}

			return true;
		}
		return false;
	}

	bool Node::OnEvent(_In_ Event& e)
	{
		switch (e.GetType())
		{
		case EventType::MouseButtonPressedEvent:
		{
			if (((MouseButtonPressedEvent&)e).GetButton() == MouseButton::Left)
				this->SetFocus();
			return true;
		}
		case EventType::SetCursorEvent:
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			return true;
		}
		}
		return false;
	}
	
}


