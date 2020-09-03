#pragma once

#include "Control.h"


namespace NPE
{
	class Node : public Control
	{
	public:
		Node(HWND parent, const NPoint& pos, const NSize& size, const NColor& color, const std::string& text);

	private:

	};
}



