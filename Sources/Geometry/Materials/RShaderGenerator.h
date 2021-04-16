#pragma once

#include "../../RBase.h"
#include "../../Graphics/Pipelines/Uniforms/RUniform.h"
#include "../../Graphics/Pipelines/RShader.h"

#include <vector>
#include <string>
#include <utility>


namespace At0::Ray
{
	class RAY_EXPORT ShaderGenerator
	{
	public:
		/**
		 * Defines the layout of a buffer uniform block.
		 */
		struct Member
		{
			std::string name;
			Shader::DataType type;
		};
		using UniformLayout = std::vector<Member>;

	public:
		ShaderGenerator() = default;

		void AddInputAttribute(uint32_t location, std::string_view name, Shader::DataType type);
		void AddOutputAttribute(uint32_t location, std::string_view name, Shader::DataType type);

		/**
		 * @param set The set number of this uniform
		 * @param blockName The name of the uniform block
		 * @param data The layout of the uniform block
		 * @param instanceName The name to identify an instance of the uniform
		 */
		void AddUniform(uint32_t set, std::string_view blockName, const UniformLayout& data,
			std::string_view instanceName);

		void AddFunction(std::string_view fName, std::string_view mainCode,
			std::string_view retType = "void", std::string_view args = "");

		std::string_view GetSource() const { return m_Source; }

	private:
		std::string m_Source =
			"#version 450 core\n#extension GL_ARB_separate_shader_objects : enable\n\n";
		uint32_t m_NextBinding = 0;
	};
}  // namespace At0::Ray
