#pragma once

#include "RMaterial.h"


#include "../../Graphics/Pipelines/Uniforms/RBufferUniform.h"


namespace At0::Ray
{
	class RAY_EXPORT ColorMaterial : public Material
	{
	public:
		template<typename... Args>
		static Ref<ColorMaterial> Create(const Material::Color& color, Args&&... args);

	private:
		ColorMaterial(const Material::Config& config);

	private:
		Float3 m_Color;
	};


	template<typename... Args>
	Ref<ColorMaterial> ColorMaterial::Create(const Material::Color& color, Args&&... args)
	{
		Material::Config config{};
		config.color = color;
		(FillConfig(config, args), ...);

		return Ref<ColorMaterial>(new ColorMaterial(config));
	}
}  // namespace At0::Ray
