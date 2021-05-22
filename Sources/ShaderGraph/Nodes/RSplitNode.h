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

	public:
		std::string GetFunctionCalls(OutputID outputID) const override;
	};
}  // namespace At0::Ray
