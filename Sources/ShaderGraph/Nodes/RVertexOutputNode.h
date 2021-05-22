#pragma once

#include "RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT VertexOutputNode : public Node
	{
	public:
		enum Connection
		{
			// Input
			Vertex
		};
	};
}  // namespace At0::Ray
