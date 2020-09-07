#include "pch.h"
#include "Button.h"
#include "GUI/Graphics/Renderer.h"

using namespace Util;


namespace GUI
{
	Button::Button(Control* parent)
		: Control(parent), m_Text(L"")
	{

	}

	bool Button::Render()
	{
		return false;
	}

	std::optional<std::pair<Util::NPoint, Util::NSize>> Button::CalculateLayout(const Util::NPoint& parentPos, const Util::NSize& parentSize)
	{
		return { {{}, {}} };
	}

}


