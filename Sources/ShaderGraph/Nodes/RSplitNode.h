#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT SplitNode : public Node
	{
	public:
		enum Connection
		{
			// Input
			Input,

			// Outputs
			R,
			G,
			B,
			A
		};
	};
}  // namespace At0::Ray
