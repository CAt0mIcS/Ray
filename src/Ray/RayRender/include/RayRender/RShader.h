#pragma once

#include "RRBase.h"

#include <string_view>

namespace ShaderConductor
{
	enum class ShaderStage : uint32_t;
	enum class ShadingLanguage : uint32_t;
}

namespace At0::Ray
{


	class RR_API Shader
	{
	public:
		static Scope<Shader> Create(std::string_view vertexFilepath, std::string_view pixelFilepath);

		virtual void Bind() = 0;

		virtual ~Shader() = default;

	protected:
		static std::string TranscompileShader(std::string_view filepath, ShaderConductor::ShaderStage stage, ShaderConductor::ShadingLanguage outputLanguage);
		static std::string ReadShaderSource(std::string_view filepath);

	protected:
		Shader() = default;
	};
}