#include "Rpch.h"
#include "RVertexNode.h"

#include "Utils/RString.h"

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

	void VertexNode::OnParentConnected(
		Node* parentNode, InputID parentConnectionID, OutputID connectionID)
	{
		switch (connectionID)
		{
		case VertexNode::ScreenSpacePosition:
			m_Attributes["inPos"] = Node::AttributeData{ "in", "vec3" };
			break;
		}
	}
}  // namespace At0::Ray
