#include "Rpch.h"
#include "RSplitNode.h"


namespace At0::Ray
{
	std::string SplitNode::GetFunctionCalls(OutputID outputID) const
	{
		const ChildNode& child = m_Children.at(SplitNode::Input);
		const std::string functionCalls = child.node->GetFunctionCalls(child.connectionPoint);

		switch (outputID)
		{
		case SplitNode::R: return String::Serialize("{0}.x", functionCalls);
		case SplitNode::G: return String::Serialize("{0}.y", functionCalls);
		case SplitNode::B: return String::Serialize("{0}.z", functionCalls);
		case SplitNode::A: return String::Serialize("{0}.w", functionCalls);
		}

		return "";
	}
}  // namespace At0::Ray
