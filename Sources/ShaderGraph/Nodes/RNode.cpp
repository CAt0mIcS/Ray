#include "Rpch.h"
#include "RNode.h"


namespace At0::Ray
{
	void Node::Connect(Ref<Node> childNode, OutputID childConnectionID, InputID connectionID)
	{
		m_Children[connectionID] = ChildNode{ childNode, childConnectionID };

		childNode->OnParentConnected(this, connectionID, childConnectionID);
		OnChildConnected(std::move(childNode), childConnectionID, connectionID);
	}

	bool Node::HasConnection(InputID connectionID) const
	{
		return m_Children.find(connectionID) != m_Children.end();
	}

	void Node::OnChildConnected(
		Ref<Node> childNode, OutputID childConnectionID, InputID connectionID)
	{
	}

	void Node::OnParentConnected(
		Node* parentNode, InputID parentConnectionID, OutputID connectionID)
	{
	}
}  // namespace At0::Ray
