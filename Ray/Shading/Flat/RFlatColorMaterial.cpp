#include "Rpch.h"
#include "RFlatColorMaterial.h"

#include "Graphics/RCodex.h"


namespace At0::Ray
{
	FlatColorMaterial::FlatColorMaterial(Layout layout, GraphicsPipeline::Layout pipelineLayout)
		: m_Layout(std::move(layout))
	{
		if (!pipelineLayout.shader)
			pipelineLayout.shader = Shader::Acquire(ChooseShaders());

		m_GraphicsPipeline = Codex::Resolve<GraphicsPipeline>(std::move(pipelineLayout));
	}

	std::vector<std::string> FlatColorMaterial::ChooseShaders()
	{
		std::vector<std::string> shaders;
		std::vector<std::string> shaderCodes;

		shaderCodes.emplace_back(ShaderFileCode::Color);

		// Sort shader codes alphabetically
		std::sort(shaderCodes.begin(), shaderCodes.end());

		std::string shaderCode =
			std::accumulate(shaderCodes.begin(), shaderCodes.end(), std::string{});
		shaders.emplace_back("Resources/Shaders/Flat" + shaderCode + ".vert");
		shaders.emplace_back("Resources/Shaders/Flat" + shaderCode + ".frag");
		return shaders;
	}
}  // namespace At0::Ray
