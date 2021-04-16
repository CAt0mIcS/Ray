#include "Rpch.h"
#include "RModelMaterial.h"

#include "Graphics/Pipelines/RGraphicsPipeline.h"


namespace At0::Ray
{
	static std::vector<std::string> GetShaders(
		bool hasDiffuseMap, bool hasSpecularMap, bool hasNormalMap)
	{
		std::vector<std::string> shaders;
		std::vector<std::string> shaderCodes;

		if (hasDiffuseMap)
			shaderCodes.emplace_back("Diff");
		if (hasSpecularMap)
			shaderCodes.emplace_back("Spec");
		if (hasNormalMap)
			shaderCodes.emplace_back("Norm");

		// Sort shader codes alphabetically
		std::sort(shaderCodes.begin(), shaderCodes.end());

		shaderCodes.emplace(shaderCodes.begin(), "_");
		std::string shaderCode =
			std::accumulate(shaderCodes.begin(), shaderCodes.end(), std::string{});

		shaders.emplace_back("Resources/Shaders/ModelShader" + shaderCode + ".vert");
		shaders.emplace_back("Resources/Shaders/ModelShader" + shaderCode + ".frag");

		return shaders;
	}

	ModelMaterial::ModelMaterial(const Material::Config& config)
		: m_DiffuseMap(config.diffuseMap), m_SpecularMap(config.specularMap),
		  m_NormalMap(config.normalMap)
	{
		bool hasDiffuseMap = config.diffuseMap.value != nullptr;
		bool hasSpecularMap = config.specularMap.value != nullptr;
		bool hasNormalMap = config.normalMap.value != nullptr;

		std::vector<std::string> shaders = GetShaders(hasDiffuseMap, hasSpecularMap, hasNormalMap);
		CreatePipeline(shaders, nullptr, config.cullMode, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			config.polygonMode);

		if (hasDiffuseMap)
		{
			AddUniform("materialDiffuse",
				MakeScope<SamplerUniform>("materialDiffuse", Shader::Stage::Fragment,
					*config.diffuseMap.value, GetGraphicsPipeline()));
		}
		if (hasSpecularMap)
		{
			AddUniform("materialSpecular",
				MakeScope<SamplerUniform>("materialSpecular", Shader::Stage::Fragment,
					*config.specularMap.value, GetGraphicsPipeline()));
		}
	}
}  // namespace At0::Ray
