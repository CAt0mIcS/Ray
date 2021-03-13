#include "Rpch.h"
#include "RMaterial.h"

#include "Graphics/RCodex.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/Images/RTexture2D.h"


namespace At0::Ray
{
	Material::Material(const std::vector<std::string_view>& shaders, const Float4& baseDiffuse,
		Ref<Texture2D> imageDiffuse, float metallic, float roughness, Ref<Texture2D> imageMaterial,
		Ref<Texture2D> imageNormal, const VertexLayout* pLayout)
		: m_BaseDiffuse(baseDiffuse), m_DiffuseImage(imageDiffuse), m_Metallic(metallic),
		  m_Roughness(roughness), m_MaterialImage(imageMaterial), m_NormalImage(imageNormal)
	{
		m_GraphicsPipeline =
			Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderPass(), shaders, pLayout);
	}

	Material::~Material() {}

	const VertexLayout& Material::GetVertexLayout() const
	{
		return m_GraphicsPipeline->GetVertexLayout();
	}
}  // namespace At0::Ray
