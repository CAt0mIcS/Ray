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
}  // namespace At0::Ray
