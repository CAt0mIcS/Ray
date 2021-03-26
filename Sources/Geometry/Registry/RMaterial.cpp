#include "Rpch.h"
#include "RMaterial.h"

#include "Graphics/RCodex.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/Images/RTexture2D.h"


namespace At0::Ray
{
	Material::Material(const std::vector<std::string>& shaders, const Float4& baseDiffuse,
		Ref<Texture2D> diffuseMap, Ref<Texture2D> specularMap, Ref<Texture2D> normalMap,
		float metallic, float roughness, const VertexLayout* pLayout)
		: m_BaseDiffuse(baseDiffuse), m_DiffuseMap(diffuseMap), m_Metallic(metallic),
		  m_Roughness(roughness), m_SpecularMap(specularMap), m_NormalMap(normalMap)
	{
		m_GraphicsPipeline = Codex::Resolve<GraphicsPipeline>(
			Graphics::Get().GetRenderPass(), shaders, pLayout, Graphics::Get().GetPipelineCache());
	}

	Material::~Material() {}

	const VertexLayout& Material::GetVertexLayout() const
	{
		return m_GraphicsPipeline->GetVertexLayout();
	}
}  // namespace At0::Ray
