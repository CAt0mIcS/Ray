#pragma once

#include "RMaterial.h"


namespace At0::Ray
{
	class RAY_EXPORT TextureMaterial : public Material
	{
	public:
		template<typename... Args>
		static Ref<TextureMaterial> Create(Ref<Texture2D> texture2D, Args&&... args);

	private:
		TextureMaterial(const Material::Config& config);

	private:
		Ref<Texture2D> m_Texture;
	};


	template<typename... Args>
	inline Ref<TextureMaterial> TextureMaterial::Create(Ref<Texture2D> texture2D, Args&&... args)
	{
		Material::Config config{};
		config.texture2D = texture2D;
		(FillConfig(config, args), ...);

		return Ref<TextureMaterial>(new TextureMaterial(config));
	}
}  // namespace At0::Ray
