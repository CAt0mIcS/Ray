#pragma once

#include "RMaterial.h"


namespace At0::Ray
{
	class RAY_EXPORT ModelMaterial : public Material
	{
	public:
		template<typename... Args>
		static Ref<ModelMaterial> Create(Args&&... args);

	private:
		ModelMaterial(const Material::Config& config);

	private:
		Ref<Texture2D> m_SpecularMap = nullptr;
		Ref<Texture2D> m_DiffuseMap = nullptr;
		Ref<Texture2D> m_NormalMap = nullptr;
	};


	template<typename... Args>
	inline Ref<ModelMaterial> ModelMaterial::Create(Args&&... args)
	{
		Material::Config config;
		(FillConfig(config, args), ...);

		return Ref<ModelMaterial>(new ModelMaterial(config));
	}
}  // namespace At0::Ray
