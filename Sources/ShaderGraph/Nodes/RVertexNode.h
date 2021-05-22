#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT VertexNode : public Node
	{
	public:
		enum Connection
		{
			// Output
			ScreenSpacePosition
		};
	};
}  // namespace At0::Ray
