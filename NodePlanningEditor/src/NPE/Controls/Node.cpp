#include "Node.h"


namespace NPE
{
	//TODO: Measure speed and test if variables are being moved instead of coppied
	Node::Node(HWND parent, const Type type, const NPoint& pos, const NSize& size, const std::string& text)
		: Control(parent, type, pos, size, text)
	{
	}
}


