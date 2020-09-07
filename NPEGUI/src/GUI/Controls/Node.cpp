#include "pch.h"
#include "Node.h"

#include "GUI/Handlers/Mouse.h"


using namespace Util;


namespace GUI
{
	//TODO: Measure speed and test if variables are being moved instead of coppied
	Node::Node(Control* parent)
		: Control(parent)
	{
	}

	bool Node::Render()
	{
		return false;
	}
	
}


