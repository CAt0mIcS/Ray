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
	};
}  // namespace At0::Ray
