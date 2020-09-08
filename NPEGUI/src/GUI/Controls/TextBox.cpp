#include "pch.h"
#include "TextBox.h"

#include "GUI/Graphics/Renderer.h"
#include "GUI/Graphics/TextRenderer.h"

/**
* QUESTION:
*	Should I be designing something like this GUI library only for this project or should I make it usable in another one?
*/


using namespace Util;

namespace GUI
{
	TextBox::TextBox(Control* parent)
		: Control(parent), m_Text(L""), m_FontFamily(L"Consolas"), m_FontSize(20)
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
			RenderText();

			return true;
		}

		return false;
	}

	void TextBox::RenderText()
	{
		NText text;
		text.text = m_Text;
		text.fontFamily = m_FontFamily;
		text.fontSize = m_FontSize;
		text.size = GetSize();
		text.pos = GetPos();

		TextRenderer::Get().RenderText(text);
	}

	std::optional<std::pair<Util::NPoint, Util::NSize>> TextBox::CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize)
	{
		return { {{}, {}} };
	}

}


