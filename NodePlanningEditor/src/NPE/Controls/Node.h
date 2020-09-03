#pragma once

#include "Control.h"


namespace NPE
{
	class Node : public Control
	{
	public:
		Node(HWND parent, const Type type, const NPoint& pos, const NSize& size, const std::string& text);

	private:

	};
}



