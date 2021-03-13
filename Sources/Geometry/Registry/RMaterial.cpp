#include "Rpch.h"
#include "RMaterial.h"

#include "Graphics/RCodex.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/Images/RTexture2D.h"


namespace At0::Ray
{
	Material::Material(const Float4& baseDiffuse, std::shared_ptr<Texture2D> imageDiffuse,
		float metallic, float roughness, std::shared_ptr<Texture2D> imageMaterial,
		std::shared_ptr<Texture2D> imageNormal, bool useTexturedShader)
		: m_BaseDiffuse(baseDiffuse), m_DiffuseImage(imageDiffuse), m_Metallic(metallic),
		  m_Roughness(roughness), m_MaterialImage(imageMaterial), m_NormalImage(imageNormal)
	{
		if (useTexturedShader)
		{
			std::vector<std::string_view> shaders{ "Resources/Shaders/TexturedShader.vert",
				"Resources/Shaders/TexturedShader.frag" };
			m_GraphicsPipeline =
				Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderPass(), shaders);
		}
		else
		{
			std::vector<std::string_view> shaders{ "Resources/Shaders/DefaultShader.vert",
				"Resources/Shaders/DefaultShader.frag" };
			m_GraphicsPipeline =
				Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderPass(), shaders);
		}
	}

	Material::Material(const std::vector<std::string_view>& shaders)
	{
		m_GraphicsPipeline =
			Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderPass(), shaders);
	}

	Material::~Material() {}

	const VertexLayout& Material::GetVertexLayout() const
	{
		return m_GraphicsPipeline->GetVertexLayout();
	}
}  // namespace At0::Ray
