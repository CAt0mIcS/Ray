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


