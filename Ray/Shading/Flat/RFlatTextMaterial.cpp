#include "Rpch.h"
#include "RFlatTextMaterial.h"

#include "Graphics/RCodex.h"
#include "Graphics/Text/RFont.h"
#include "Graphics/Pipelines/Shader/RShader.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"


namespace At0::Ray
{
	FlatTextMaterial::FlatTextMaterial(Layout layout, GraphicsPipeline::Layout pipelineLayout)
		: m_Layout(std::move(layout))
	{
		pipelineLayout.shader = Shader::FromGlsl(
			{ "Resources/Shaders/Flat_Diff.vert", "Resources/Shaders/Flat_Diff.frag" });
		pipelineLayout.cullMode = VK_CULL_MODE_NONE;

		m_GraphicsPipeline = Codex::Resolve<GraphicsPipeline>(std::move(pipelineLayout));
	}
}  // namespace At0::Ray
