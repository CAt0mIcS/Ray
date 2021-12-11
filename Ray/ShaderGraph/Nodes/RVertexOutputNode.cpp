#include "Rpch.h"
#include "RVertexOutputNode.h"


namespace At0::Ray
{
	std::string VertexOutputNode::GetFunctionCalls(OutputID outputID) const
	{
		const ChildNode& child = m_Children.at(VertexOutputNode::Vertex);
		return String::Serialize(
			"gl_Position = {0}", child.node->GetFunctionCalls(child.connectionPoint));
	}
}  // namespace At0::Ray
