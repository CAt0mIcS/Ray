#pragma once

#include "Nodes/RNode.h"
#include "../Graphics/Pipelines/Shader/RShader.h"


namespace At0::Ray
{
	class MeshRenderer;
	class Texture2D;

	class RAY_EXPORT ShaderGenerator
	{
	public:
		std::string GenerateVertexShader(std::vector<Ref<Node>> rootNodes);
		std::string GenerateFragmentShader(std::vector<Ref<Node>> rootNodes);

		void AddUniforms(MeshRenderer& renderer);

	private:
		/**
		 * {0}: Input and output attributes
		 * {1}: Uniforms
		 * {2}: Functions
		 * {3}: Main shader code
		 * @returns String ready for serialization of args
		 */
		std::string GenerateTemplate() const;

		std::string GetUniforms(const Node& rootNode, std::string_view uniforms, ShaderStage stage);

	private:
		uint32_t m_VSNextInputAttributeLocation = 0;
		uint32_t m_VSNextOutputAttributeLocation = 0;

		uint32_t m_NextBinding = 1;

		uint32_t m_FSNextInputAttributeLocation = 0;
		uint32_t m_FSNextOutputAttributeLocation = 0;

		struct UniformData
		{
			std::string name;
			ShaderStage stage;
			Shader::UniformBlocks::Type type;
			Ref<Texture2D> texture;
		};
		std::vector<UniformData> m_Uniforms;
	};
}  // namespace At0::Ray
