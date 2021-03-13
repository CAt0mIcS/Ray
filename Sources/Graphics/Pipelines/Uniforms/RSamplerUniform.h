#pragma once

#include "RUniform.h"


namespace At0::Ray
{
	class Texture2D;
	class Pipeline;

	class RAY_EXPORT SamplerUniform : public Uniform
	{
	public:
		SamplerUniform(std::string_view uniformName, Shader::Stage stage, const Texture2D& texture,
			const Pipeline& pipeline);
		~SamplerUniform();

	private:
	};
}  // namespace At0::Ray
