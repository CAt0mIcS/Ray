#include "rlpch.h"
#include "RenderedWidget.h"


namespace Zeal::Reyal
{
	RenderedWidget::RenderedWidget(const std::wstring_view name, WindowRenderer& renderer, Widget* parent)
		: Widget(name, parent), m_Renderer(renderer)
	{

	}
}


