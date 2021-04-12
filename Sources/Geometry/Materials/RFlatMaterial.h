#pragma once

#include "RMaterial.h"


#include "../../Graphics/Pipelines/Uniforms/RBufferUniform.h"


namespace At0::Ray
{
	class RAY_EXPORT FlatMaterial : public Material
	{
	public:
		template<typename... Args>
		static Ref<FlatMaterial> Create(Args&&... args);

	private:
		FlatMaterial(const Material::Config& config);

	private:
		Float3 m_Color;
	};


	template<typename... Args>
	Ref<FlatMaterial> FlatMaterial::Create(Args&&... args)
	{
		Material::Config config{};
		(FillConfig(config, args), ...);

		return Ref<FlatMaterial>(new FlatMaterial(config));
	}
}  // namespace At0::Ray
