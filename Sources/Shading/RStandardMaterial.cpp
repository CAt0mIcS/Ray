#include "Rpch.h"
#include "RStandardMaterial.h"


namespace At0::Ray
{
	StandardMaterial::StandardMaterial(Ref<Texture2D> albedoMap, Ref<Texture2D> diffuseMap,
		Ref<Texture2D> specularMap, Ref<Texture2D> normalMap, Float4 color, float metallic)
		: m_AlbedoMap(std::move(albedoMap)), m_DiffuseMap(std::move(diffuseMap)),
		  m_SpecularMap(std::move(specularMap)), m_NormalMap(std::move(normalMap)),
		  m_Color(std::move(color)), m_Metallic(metallic)
	{
	}

	void StandardMaterial::SetAlbedoMap(Ref<Texture2D> albedo)
	{
		// If we enable/disable albedo mapping we need a different shader
		if ((albedo && !m_AlbedoMap) || (!albedo && m_AlbedoMap))
		{
			m_AlbedoMap = albedo;
			RecreatePipeline();
		}
		else
			m_AlbedoMap = albedo;
	}

	void StandardMaterial::SetDiffuseMap(Ref<Texture2D> diffuse)
	{
		// If we enable/disable diffuse mapping we need a different shader
		if ((diffuse && !m_DiffuseMap) || (!diffuse && m_DiffuseMap))
		{
			m_DiffuseMap = diffuse;
			RecreatePipeline();
		}
		else
			m_DiffuseMap = diffuse;
	}

	void StandardMaterial::SetSpecularMap(Ref<Texture2D> specular)
	{
		// If we enable/disable specular mapping we need a different shader
		if ((specular && !m_SpecularMap) || (!specular && m_SpecularMap))
		{
			m_SpecularMap = specular;
			RecreatePipeline();
		}
		else
			m_SpecularMap = specular;
	}

	void StandardMaterial::SetNormalMap(Ref<Texture2D> normal)
	{
		// If we enable/disable normal mapping we need a different shader
		if ((normal && !m_NormalMap) || (!normal && m_NormalMap))
		{
			m_NormalMap = normal;
			RecreatePipeline();
		}
		else
			m_NormalMap = normal;
	}

	void StandardMaterial::RecreatePipeline() {}
}  // namespace At0::Ray
