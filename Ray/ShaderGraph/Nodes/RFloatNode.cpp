#include "RFloatNode.h"


namespace At0::Ray
{
	FloatNode::FloatNode(float value) : m_Value(value) {}

	std::string FloatNode::GetFunctionCalls(OutputID outputID) const
	{
		if (outputID == FloatNode::Result)
			return std::to_string(m_Value);

		return "";
	}
}  // namespace At0::Ray
