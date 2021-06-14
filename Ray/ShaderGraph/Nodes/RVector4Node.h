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

	public:
		std::string GetFunctionCalls(OutputID outputID) const override;
	};
}  // namespace At0::Ray
