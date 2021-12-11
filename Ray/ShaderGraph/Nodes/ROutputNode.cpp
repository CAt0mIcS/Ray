#include "Rpch.h"
#include "ROutputNode.h"

namespace At0::Ray
{
	OutputNode::OutputNode(std::string_view outputType, std::string_view outputName)
		: m_OutputType(outputType), m_OutputName(outputName)
	{
	}

	std::string OutputNode::GetFunctionCalls(OutputID outputID) const
	{
		const ChildNode& child = m_Children.at(OutputNode::Input);
		return String::Serialize(
			"{0} = {1}", m_OutputName, child.node->GetFunctionCalls(child.connectionPoint));
	}

	void OutputNode::OnChildConnected(
		Ref<Node> childNode, OutputID childConnectionID, InputID connectionID)
	{
		m_Attributes[m_OutputName] = AttributeData{ "out", m_OutputType };
	}
}  // namespace At0::Ray
