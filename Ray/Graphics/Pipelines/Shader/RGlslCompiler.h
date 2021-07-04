#pragma once

#include "Ray/RBase.h"
#include "RShaderReflection.h"

#include <vulkan/vulkan_core.h>
#include <RayRenderer/Pipeline/RShader.h>

namespace glslang
{
	class TProgram;
}  // namespace glslang

namespace At0::Ray
{
	enum class ShaderStage;

	class RAY_EXPORT GlslCompiler
	{
	public:
		GlslCompiler(const std::vector<std::string>& shaders);

		const auto& GetReflections() const { return m_Reflections; }
		auto&& AcquireReflections() { return std::move(m_Reflections); }

		const auto& GetShaderModules() const { return m_ShaderModules; }
		auto&& AcquireShaderModules() { return std::move(m_ShaderModules); }

	private:
		RrShaderModule CreateShaderModule(std::string_view preamble, ShaderStage shaderStage);
		void LoadUniform(const glslang::TProgram& program, ShaderStage stageFlag, int32_t i);
		void LoadUniformBlock(const glslang::TProgram& program, ShaderStage stageFlag, int32_t i);
		void LoadAttribute(const glslang::TProgram& program, ShaderStage stageFlag, int32_t i);

	private:
		std::unordered_map<ShaderStage, std::string> m_Shaders;
		std::unordered_map<ShaderStage, RrShaderModule> m_ShaderModules;
		std::unordered_map<ShaderStage, ShaderReflection> m_Reflections;
	};
}  // namespace At0::Ray
