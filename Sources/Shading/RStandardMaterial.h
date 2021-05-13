#pragma once

#include "RMaterial.h"
#include "../Core/RMath.h"


namespace At0::Ray
{
	class Texture2D;

	/**
	 * Material which has all basic properties like
	 * Color, AlbedoMap, NormalMap, Metallic/SpecularMap, ...
	 */
	class RAY_EXPORT StandardMaterial : public Material
	{
	public:
		StandardMaterial(Ref<Texture2D> albedoMap = nullptr, Ref<Texture2D> diffuseMap = nullptr,
			Ref<Texture2D> specularMap = nullptr, Ref<Texture2D> normalMap = nullptr,
			Float4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, float metallic = 1.0f);

		void SetAlbedoMap(Ref<Texture2D> albedo);
		void SetDiffuseMap(Ref<Texture2D> diffuse);
		void SetSpecularMap(Ref<Texture2D> specular);
		void SetNormalMap(Ref<Texture2D> normal);
		void SetColor(Float4 color) { m_Color = std::move(color); }
		void SetMetallic(float metallic) { m_Metallic = metallic; }

	private:
		void RecreatePipeline();

	private:
		Ref<Texture2D> m_AlbedoMap;
		Ref<Texture2D> m_DiffuseMap;
		Ref<Texture2D> m_SpecularMap;
		Ref<Texture2D> m_NormalMap;
		Float4 m_Color;

		float m_Metallic = 1.0f;

		// RAY_TODO: Emission here
	};
}  // namespace At0::Ray
