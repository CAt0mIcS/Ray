#pragma once

#include "Nodes/RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT VertexShaderGenerator
	{
	public:
		std::string Generate(std::vector<Ref<Node>> rootNodes);

	private:
		/**
		 * {0}: Input and output attributes
		 * {1}: Uniforms
		 * {2}: Functions
		 * {3}: Main shader code
		 * @returns String ready for serialization of args
		 */
		std::string GenerateTemplate() const;
	};
}  // namespace At0::Ray
