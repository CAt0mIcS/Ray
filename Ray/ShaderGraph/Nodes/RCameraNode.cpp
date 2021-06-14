#include "Rpch.h"
#include "RCameraNode.h"

namespace At0::Ray
{
	std::string CameraNode::GetFunctionCalls(OutputID outputID) const
	{
		if (outputID == CameraNode::Projection)
			return "uPerSceneData.Proj";
		else if (outputID == CameraNode::View)
			return "uPerSceneData.View";

		return "";
	}

	void CameraNode::OnParentConnected(
		Node* parentNode, InputID parentConnectionID, OutputID connectionID)
	{
		m_BufferUniforms["PerSceneData"].emplace_back("mat4", "View");
		m_BufferUniforms["PerSceneData"].emplace_back("mat4", "Proj");
	}
}  // namespace At0::Ray
