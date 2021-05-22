#include "Rpch.h"
#include "RMultiplyNode.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	std::string MultiplyNode::GetFunctionCalls(OutputID outputID) const
	{
		const ChildNode& childLeft = m_Children.at(MultiplyNode::Left);
		const ChildNode& childRight = m_Children.at(MultiplyNode::Right);

		if (outputID == MultiplyNode::Result)
			return String::Serialize("MultiplyTechnique({0}, {1})",
				childLeft.node->GetFunctionCalls(childLeft.connectionPoint),
				childRight.node->GetFunctionCalls(childRight.connectionPoint));

		return "";
	}
}  // namespace At0::Ray
