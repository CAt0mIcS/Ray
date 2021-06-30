#include "Rpch.h"
#include "RSampler2DNode.h"

#include "Ray/Utils/RString.h"


namespace At0::Ray
{
	uint32_t Sampler2DNode::s_SamplerID = 0;

	Sampler2DNode::Sampler2DNode(std::string_view samplerName)
	{
		if (samplerName.empty())
			m_Name = "sampler2D_" + std::to_string(s_SamplerID++);
	}

	std::string Sampler2DNode::GetFunctionCalls(OutputID outputID) const
	{
		const ChildNode& childUV = m_Children.at(Sampler2DNode::UV);

		if (outputID == Sampler2DNode::Result)
			return String::Serialize("texture({0}, {1})", m_Name,
				childUV.node->GetFunctionCalls(childUV.connectionPoint));

		return "";
	}

	void Sampler2DNode::OnParentConnected(
		Node* parentNode, InputID parentConnectionID, OutputID connectionID)
	{
		m_SamplerUniforms[m_Name] = "sampler2D";
	}
}  // namespace At0::Ray
