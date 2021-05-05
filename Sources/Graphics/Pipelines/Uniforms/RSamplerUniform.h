#pragma once

#include "RUniform.h"


namespace At0::Ray
{
	class Texture2D;
	class Pipeline;

	class RAY_EXPORT SamplerUniform : public Uniform
	{
	public:
		SamplerUniform(std::string_view uniformName, Shader::Stage stage, Ref<Texture2D> texture,
			const Pipeline& pipeline);
		~SamplerUniform();

		SamplerUniform& operator=(const SamplerUniform& other);
		SamplerUniform(const SamplerUniform& other);

		const Texture2D& GetTexture() const { return *m_Texture; }
		void SetTexture(Ref<Texture2D> tex);

	private:
		void Setup();

	private:
		Ref<Texture2D> m_Texture;
		uint32_t m_Set = 0;
	};
}  // namespace At0::Ray
