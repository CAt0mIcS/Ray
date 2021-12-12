#include "RTransformationMatrixNode.h"

namespace At0::Ray
{
	std::string TransformationMatrixNode::GetFunctionCalls(OutputID outputID) const
	{
		switch (outputID)
		{
		case TransformationMatrixNode::Model: return "uPerObjectData.Model";
		}

		return "";
	}

	void TransformationMatrixNode::OnParentConnected(
		Node* parentNode, InputID parentConnectionID, OutputID connectionID)
	{
		switch (connectionID)
		{
		case TransformationMatrixNode::Model:
			m_BufferUniforms["PerObjectData"].emplace_back("mat4", "Model");
			break;
		}
	}
}  // namespace At0::Ray
