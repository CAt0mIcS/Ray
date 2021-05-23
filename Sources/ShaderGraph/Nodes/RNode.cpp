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

	std::string Node::GetAttributes(
		uint32_t& inputLocation, uint32_t& outputLocation, OutputID outputID) const
	{
		std::string merged;
		for (const auto& [connection, child] : m_Children)
		{
			std::string attribs =
				child.node->GetAttributes(inputLocation, outputLocation, child.connectionPoint);
			if (merged.find(attribs) == std::string::npos)
			{
				// Check if only location is different
				std::string noLocation = attribs.substr(
					strlen("layout(location = ") + 2);	// max number of locations: 99

				if (merged.find(noLocation) == std::string::npos)
					merged += attribs;
				else
				{
					// Decrement input/output location
					if (attribs.find(" in ") != std::string::npos)
						--inputLocation;
					else
						--outputLocation;
				}
			}
		}
		return merged;
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
