#pragma once

#include "RlBase.h"

#include "ZWidget.h"

namespace Zeal::Reyal
{
	class WindowRenderer;

	class RL_API RenderedWidget : public Widget
	{
	protected:
		RenderedWidget(const std::wstring_view name, WindowRenderer& renderer, _In_opt_ Widget* parent);

	protected:
		WindowRenderer& m_Renderer;
	};
}



