#pragma once

#include "RShaderGenerator.h"


namespace At0::Ray
{
	class RAY_EXPORT FlatShaderGenerator : public ShaderGenerator
	{
	public:
		enum Connection
		{
			Color
		};

	public:
		FlatShaderGenerator() = default;

		std::vector<std::string> Generate() const override;

	private:
		std::string GenerateVertexShader(std::string shaderTemplate) const;
	};
}  // namespace At0::Ray
