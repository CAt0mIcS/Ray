#include "RLayer.h"

#include "Scene/RScene.h"
#include "Devices/RWindow.h"
#include "Graphics/Pipelines/Shader/RShader.h"

namespace At0::Ray
{
	Layer::Layer(Scene& scene) : m_Scene(&scene) {}

	const Window& Layer::GetWindow() const
	{
		return GetScene().GetMainWindow();
	}


	const ResourceManager& Layer::GetResourceManager() const
	{
		return m_Scene->GetResourceManager();
	}

	GraphicsPipeline::Builder Layer::PipelineBuilder()
	{
		return GraphicsPipeline::Builder();
	}

	Ref<Shader> Layer::LoadShaderFromSourceFile(
		const std::vector<std::string>& shaders, const std::vector<std::string>& reflections)
	{
		std::string tag = std::accumulate(shaders.begin(), shaders.end(), std::string{});
		if (auto stored = GetResourceManager().Get<Shader>(tag); stored)
			return stored;

		auto shader = Shader::FromSourceFile(shaders, reflections);
		return GetResourceManager().Emplace<Shader>(tag, std::move(shader));
	}

	Ref<Shader> Layer::LoadShaderFromCompiledFile(
		const std::vector<std::string>& shaders, const std::vector<std::string>& reflections)
	{
		std::string tag = std::accumulate(shaders.begin(), shaders.end(), std::string{});
		if (auto stored = GetResourceManager().Get<Shader>(tag); stored)
			return stored;

		auto shader = Shader::FromCompiledFile(shaders, reflections);
		return GetResourceManager().Emplace<Shader>(tag, std::move(shader));
	}

	Ref<Shader> Layer::LoadShaderFromString(const std::vector<std::string>& shaders,
		const std::vector<ShaderStage>& stageOrder, const std::vector<std::string>& reflections)
	{
		return Shader::FromSourceString(shaders, stageOrder, reflections);
	}

	Material::Builder Layer::MaterialBuilder(Ref<GraphicsPipeline> pipeline)
	{
		return Material::Builder(pipeline);
	}
}  // namespace At0::Ray