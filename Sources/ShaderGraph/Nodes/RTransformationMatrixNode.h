#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT TransformationMatrix : public Node
	{
	public:
		enum Connection
		{
			// Output
			Model
		};
	};
}  // namespace At0::Ray
