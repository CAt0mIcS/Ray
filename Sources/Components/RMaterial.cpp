#include "Rpch.h"
#include "RMaterial.h"

#include "Graphics/Core/RCodex.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/RenderPass/RRenderPass.h"


namespace At0::Ray
{
	const VertexLayout& Material::GetVertexLayout() const
	{
		return m_GraphicsPipeline->GetVertexLayout();
	}

	Material Material::Default()
	{
		std::vector<std::string_view> shaders = { "Resources/Shaders/DefaultShader.vert",
			"Resources/Shaders/DefaultShader.frag" };
		return { Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderPass(), shaders) };
	}

	Material::Material(Ref<GraphicsPipeline> pipeline) : m_GraphicsPipeline(std::move(pipeline)) {}
}  // namespace At0::Ray