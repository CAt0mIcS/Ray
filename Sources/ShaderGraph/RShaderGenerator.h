#pragma once

#include "Nodes/RNode.h"


namespace At0::Ray
{
	class RAY_EXPORT ShaderGenerator
	{
	public:
		std::string GenerateVertexShader(std::vector<Ref<Node>> rootNodes);
		std::string GenerateFragmentShader(std::vector<Ref<Node>> rootNodes);

	private:
		/**
		 * {0}: Input and output attributes
		 * {1}: Uniforms
		 * {2}: Functions
		 * {3}: Main shader code
		 * @returns String ready for serialization of args
		 */
		std::string GenerateTemplate() const;

	private:
		uint32_t m_VSNextInputAttributeLocation = 0;
		uint32_t m_VSNextOutputAttributeLocation = 0;

		uint32_t m_NextBinding = 1;

		uint32_t m_FSNextInputAttributeLocation = 0;
		uint32_t m_FSNextOutputAttributeLocation = 0;
	};
}  // namespace At0::Ray
