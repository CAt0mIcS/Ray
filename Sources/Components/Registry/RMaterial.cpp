#include "Rpch.h"
#include "RMaterial.h"

#include "Graphics/RCodex.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/RenderPass/RRenderPass.h"


namespace At0::Ray
{
	const VertexLayout& Material::GetVertexLayout() const
	{
		return m_GraphicsPipeline->GetVertexLayout();
	}

	void Material::Create()
	{
		std::vector<std::string_view> shaders{ "Resources/Shaders/DefaultShader.vert",
			"Resources/Shaders/DefaultShader.frag" };
		m_GraphicsPipeline =
			Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderPass(), shaders);
	}


}  // namespace At0::Ray
