#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT MultiplyNode : public Node
	{
	public:
		enum Connection
		{
			// Inputs
			Left,
			Right,

			// Output
			Result
		};
	};
}  // namespace At0::Ray
