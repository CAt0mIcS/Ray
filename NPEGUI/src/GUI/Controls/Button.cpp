#include "pch.h"
#include "Button.h"
#include "GUI/Graphics/Renderer.h"

#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"


namespace GUI
{
	Button::Button(_In_opt_ Control* parent)
		: Control(Type::Button, parent), m_Text(L"")
	{
	}

	bool Button::Render()
	{
		if (this->IsInWindow())
		{
			const float max = std::max(GetSize().width, GetSize().height);
			Renderer::Get().RenderRoundedRect(GetPos(), GetSize(), GetColor(), max / 5.0f, max / 5.0f);

			for (auto* child : m_Children)
			{
				child->Render();
			}
	
			return true;
		}
		return false;
	}

	bool Button::OnEvent(_In_ Event& e)
	{
		switch (e.GetType())
		{
		case EventType::MouseButtonPressedEvent:
		{
			if (((MouseButtonPressedEvent&)e).GetButton() == MouseButton::Left)
				this->SetFocus();

			//To remove caret
			Renderer::Get().BeginDraw();
			m_Parent->Render();
			Renderer::Get().EndDraw();

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

	std::optional<std::pair<Util::NPoint, Util::NSize>> Button::CalculateLayout(_In_ const Util::NPoint& parentPos, _In_ const Util::NSize& parentSize)
	{
		float middleOfNodeX = parentPos.x + (parentSize.width / 2);

		float btnWidthAndHeight = std::min(parentSize.width, parentSize.height) / 12;
		float btnX = middleOfNodeX - (btnWidthAndHeight / 2);

		float yOffsetBtn = parentSize.height / 56;
		float btnY = parentPos.y + yOffsetBtn;

		return { {{btnX, btnY}, {btnWidthAndHeight, btnWidthAndHeight}} };
	}

}


