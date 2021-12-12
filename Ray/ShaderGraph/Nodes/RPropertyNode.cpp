#include "RPropertyNode.h"


namespace At0::Ray
{
	PropertyNode::PropertyNode(
		std::string_view uBlockName, std::string_view uniformType, std::string_view uniformName)
		: m_UniformBlockName(uBlockName), m_Uniform({ uniformType.data(), uniformName.data() })
	{
	}

	std::string PropertyNode::GetFunctionCalls(OutputID outputID) const
	{
		if (outputID == PropertyNode::Result)
		{
			return String::Serialize("u{0}.{1}", m_UniformBlockName, m_Uniform.second);
		}

		return "";
	}

	void PropertyNode::OnParentConnected(
		Node* parentNode, InputID parentConnectionID, OutputID connectionID)
	{
		m_BufferUniforms[m_UniformBlockName].emplace_back(m_Uniform);
	}
}  // namespace At0::Ray
