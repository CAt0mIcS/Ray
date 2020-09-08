#include "pch.h"
#include "TextBox.h"

#include "GUI/Graphics/Renderer.h"

/**
* QUESTION:
*	Should I be designing something like this GUI library only for this project or should I make it usable in another one?
*/


using namespace Util;

namespace GUI
{
	TextBox::TextBox(Control* parent)
		: Control(parent), m_Text(L"")
	{
	}

	bool TextBox::Render()
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

	bool TextBox::RenderText()
	{
		return false;
	}

	std::optional<std::pair<Util::NPoint, Util::NSize>> TextBox::CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize)
	{
		return { {{}, {}} };
	}

}


