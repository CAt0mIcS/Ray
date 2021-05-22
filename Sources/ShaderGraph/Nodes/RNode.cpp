#include "Rpch.h"
#include "RNode.h"


namespace At0::Ray
{
	void Node::Connect(Ref<Node> childNode, uint32_t childPointID, uint32_t pointID)
	{
		m_Children[pointID] = ChildNode{ std::move(childNode), childPointID };
	}
}  // namespace At0::Ray
