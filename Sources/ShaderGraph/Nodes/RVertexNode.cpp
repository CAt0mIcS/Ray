#include "Rpch.h"
#include "RVertexNode.h"

#include "Utils/RString.h"

namespace At0::Ray
{
	std::string VertexNode::GetAttributes(
		uint32_t& inputLocation, uint32_t& outputLocation, OutputID outputID) const
	{
		if (outputID == VertexNode::ScreenSpacePosition)
			return String::Serialize("layout(location = {0}) in vec3 inPos;", inputLocation++);

		return "";
	}

	std::string VertexNode::GetFunctionCalls(OutputID outputID) const
	{
		switch (outputID)
		{
		case VertexNode::ScreenSpacePosition: return "inPos";
		}

		return "";
	}
}  // namespace At0::Ray
