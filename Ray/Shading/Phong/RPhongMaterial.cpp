#include "Rpch.h"
#include "RPhongMaterial.h"

#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/RCodex.h"
#include "RPhongMaterial.h"


namespace At0::Ray
{
	PhongMaterial::PhongMaterial(Layout layout, GraphicsPipeline::Layout pipelineLayout)
		: m_Layout(std::move(layout))
	{

		if (!pipelineLayout.shader)
			pipelineLayout.shader = Shader::Acquire(ChooseShaders());

		m_GraphicsPipeline = Codex::Resolve<GraphicsPipeline>(std::move(pipelineLayout));
	}

	std::vector<std::string> PhongMaterial::ChooseShaders()
	{
		std::vector<std::string> shaders;
		std::vector<std::string> shaderCodes;

		if (m_Layout.albedoMap)
			shaderCodes.emplace_back(ShaderFileCode::AlbedoMap);
		if (m_Layout.diffuseMap)
			shaderCodes.emplace_back(ShaderFileCode::DiffuseMap);
		else
			shaderCodes.emplace_back(ShaderFileCode::Metallic);
		if (m_Layout.specularMap)
			shaderCodes.emplace_back(ShaderFileCode::SpecularMap);
		if (m_Layout.normalMap)
			shaderCodes.emplace_back(ShaderFileCode::NormalMap);
		if (m_Layout.heightMap)
			shaderCodes.emplace_back(ShaderFileCode::HeightMap);
		if (m_Layout.occlusion)
			shaderCodes.emplace_back(ShaderFileCode::Occlusion);
		if (m_Layout.detailMask)
			shaderCodes.emplace_back(ShaderFileCode::DetailMask);

		if (m_Layout.emission.enabled)
			shaderCodes.emplace_back(ShaderFileCode::Emission);

		// Sort shader codes alphabetically
		std::sort(shaderCodes.begin(), shaderCodes.end());

		if (shaderCodes.empty())
		{
			shaders.emplace_back("Resources/Shaders/Phong.vert");
			shaders.emplace_back("Resources/Shaders/Phong.frag");
			return shaders;
		}

		std::string shaderCode =
			std::accumulate(shaderCodes.begin(), shaderCodes.end(), std::string{});
		shaders.emplace_back("Resources/Shaders/Phong" + shaderCode + ".vert");
		shaders.emplace_back("Resources/Shaders/Phong" + shaderCode + ".frag");

		return shaders;
	}
}  // namespace At0::Ray
