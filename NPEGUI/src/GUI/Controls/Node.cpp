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
			const float roundAngle = m_Size.width / 5.0f;
			Renderer::Get().RenderRoundedRect(GetPos(), GetSize(), GetColor(), roundAngle, roundAngle);

			constexpr float minScale = 0.15f;
			for (auto* child : m_Children)
			{
				auto& scale = Renderer::Get().GetScale();
				if (scale.width > minScale && scale.height > minScale)
				{
					child->Render();
				}
				else if (scale.width <= minScale && scale.height <= minScale && child->GetType() == Type::Button)
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


