#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT CameraNode : public Node
	{
	public:
		enum Connection
		{
			// Outputs/Inputs
			Projection,
			View
		};
	};
}  // namespace At0::Ray
