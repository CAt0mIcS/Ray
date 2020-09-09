#include "pch.h"
#include "Button.h"
#include "GUI/Graphics/Renderer.h"

#include "GUI/Events/MouseEvent.h"
#include "GUI/Events/KeyboardEvent.h"

using namespace Util;


namespace GUI
{
	Button::Button(Control* parent)
		: Control(parent), m_Text(L"")
	{

	}

	bool Button::Render()
	{
		if (this->IsInWindow())
		{
			for (auto* child : m_Children)
			{
				child->Render();
			}

			const float max = std::max(GetSize().width, GetSize().height);
			Renderer::Get().RenderRoundedRect(GetPos(), GetSize(), GetColor(), max / 5.0f, max / 5.0f);
	
			return true;
		}
		return false;
	}

	bool Button::OnEvent(Event& e)
	{
		return false;
	}

	std::optional<std::pair<Util::NPoint, Util::NSize>> Button::CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize)
	{
		return { {{}, {}} };
	}

}


