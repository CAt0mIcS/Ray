#include "Rpch.h"
#include "RMaterial.h"

#include "Graphics/RCodex.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/RenderPass/RRenderPass.h"


namespace At0::Ray
{
	Material::Material(const Float4& baseDiffuse, std::shared_ptr<Image2D> imageDiffuse,
		float metallic, float roughness, std::shared_ptr<Image2D> imageMaterial,
		std::shared_ptr<Image2D> imageNormal)
	{
		std::vector<std::string_view> shaders{ "Resources/Shaders/DefaultShader.vert",
			"Resources/Shaders/DefaultShader.frag" };

		m_GraphicsPipeline =
			Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderPass(), shaders);
	}

	const VertexLayout& Material::GetVertexLayout() const
	{
		return m_GraphicsPipeline->GetVertexLayout();
	}
}  // namespace At0::Ray
