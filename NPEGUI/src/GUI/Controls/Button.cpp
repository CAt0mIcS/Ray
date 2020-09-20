#include "pch.h"
#include "Button.h"
#include "GUI/Graphics/Renderer.h"

#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"


namespace GUI
{
	Button::Button(_In_opt_ Control* parent)
		: Control(parent), m_Text(L"")
	{
		SetType(Type::Button);
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
		return false;
	}

	std::optional<std::pair<Util::NPoint, Util::NSize>> Button::CalculateLayout(_In_ const Util::NPoint& parentPos, _In_ const Util::NSize& parentSize)
	{
		return { {{}, {}} };
	}

}


