#include "Rpch.h"
#include "RTransformationMatrixNode.h"

namespace At0::Ray
{
	std::string TransformationMatrixNode::GetFunctionCalls(OutputID outputID) const
	{
		if (outputID == TransformationMatrixNode::Model)
			return "uPerObjectData.Model";

		return "";
	}
}  // namespace At0::Ray
