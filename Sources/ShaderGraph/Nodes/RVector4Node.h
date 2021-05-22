#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT Vector4Node : public Node
	{
	public:
		enum Connection
		{
			// Inputs
			R,
			G,
			B,
			A,

			// Output
			Result
		};
	};
}  // namespace At0::Ray
