#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT TransformationMatrixNode : public Node
	{
	public:
		enum Connection
		{
			// Output
			Model
		};

	public:
		std::string GetFunctionCalls(OutputID outputID) const override;
	};
}  // namespace At0::Ray
