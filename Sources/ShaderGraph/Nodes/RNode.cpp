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

	std::unordered_map<std::string, Node::AttributeData> Node::GetAttributes(
		OutputID outputID) const
	{
		std::unordered_map<std::string, AttributeData> data = m_Attributes;
		for (const auto& [connection, child] : m_Children)
		{
			auto childData = child.node->GetAttributes(child.connectionPoint);
			for (const auto& [attribName, attribData] : childData)
				data[attribName] = attribData;
		}
		return data;
	}

	std::string Node::GetFunctionCalls(OutputID outputID) const
	{
		std::string merged;
		for (const auto& [connection, child] : m_Children)
		{
			std::string fnCalls = child.node->GetFunctionCalls(child.connectionPoint);
			if (merged.find(fnCalls) == std::string::npos)
				merged += fnCalls;
		}
		return merged;
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
