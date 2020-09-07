#include "pch.h"
#include "TextBox.h"


using namespace Util;

namespace GUI
{
	TextBox::TextBox(Control* parent)
		: Control(parent), m_Text(L"")
	{
	}

	bool TextBox::Render()
	{
		return false;
	}

	bool TextBoxRenderText()
	{
		return false;
	}

	std::optional<std::pair<Util::NPoint, Util::NSize>> TextBox::CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize)
	{
		return { {{}, {}} };
	}

}


