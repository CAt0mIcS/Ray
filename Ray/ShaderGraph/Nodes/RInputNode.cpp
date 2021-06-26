#include "Rpch.h"
#include "RInputNode.h"

#include "ROutputNode.h"

#include "RayBase/RString.h"

namespace At0::Ray
{
	InputNode::InputNode(std::string_view inputType, std::string_view inputName)
		: m_InputType(inputType), m_InputName(inputName)
	{
	}

	InputNode::InputNode(const OutputNode& outputNode, std::string_view inputName)
		: m_InputType(outputNode.GetType()), m_InputName(inputName)
	{
	}

	std::string InputNode::GetFunctionCalls(OutputID outputID) const
	{
		switch (outputID)
		{
		case InputNode::Result: return m_InputName;
		}

		return "";
	}

	void InputNode::OnParentConnected(
		Node* parentNode, InputID parentConnectionID, OutputID connectionID)
	{
		switch (connectionID)
		{
		case InputNode::Result:
			m_Attributes[m_InputName] = Node::AttributeData{ "in", m_InputType };
			break;
		}
	}
}  // namespace At0::Ray
