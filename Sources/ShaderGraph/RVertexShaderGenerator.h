#pragma once

#include "Nodes/RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT VertexShaderGenerator
	{
	public:
		std::string Generate(std::vector<Ref<Node>> rootNodes);
	};
}  // namespace At0::Ray
