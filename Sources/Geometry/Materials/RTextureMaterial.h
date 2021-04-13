#pragma once

#include "RMaterial.h"


namespace At0::Ray
{
	class RAY_EXPORT TextureMaterial : public Material
	{
	public:
		template<typename... Args>
		static Ref<TextureMaterial> Create(Args&&... args);

	private:
		TextureMaterial(const Material::Config& config);

	private:
		Ref<Texture2D> m_Texture;
	};


	template<typename... Args>
	inline Ref<TextureMaterial> TextureMaterial::Create(Args&&... args)
	{
		Material::Config config{};
		(FillConfig(config, args), ...);

		return Ref<TextureMaterial>(new TextureMaterial(config));
	}
}  // namespace At0::Ray
