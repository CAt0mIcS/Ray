#include "Rpch.h"
#include "RVertexNode.h"

namespace At0::Ray
{
	std::string VertexNode::GetFunctionCalls(OutputID outputID) const
	{
		switch (outputID)
		{
		case VertexNode::ScreenSpacePosition: return "inPos";
		}

		return "";
	}
}  // namespace At0::Ray
