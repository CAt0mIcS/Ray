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
		std::string GenerateVertexShader(
			std::string shaderTemplate, std::string fragmentShaderInputs) const;

		static void AddAttributes(
			std::string fragmentAttributes, std::string& attributes, std::string& main);
	};
}  // namespace At0::Ray
