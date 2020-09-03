#include "Node.h"


namespace NPE
{
	//TODO: Measure speed and test if variables are being moved instead of coppied
	Node::Node(HWND parent, const NPoint& pos, const NSize& size, const NColor& color, const std::string& text)
		: Control(parent, Control::Type::Node, pos, size, color, text)
	{
	}
}


