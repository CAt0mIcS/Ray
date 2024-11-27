#include "Layer.h"

#include "Scene/RScene.h"
#include "Graphics/Pipelines/Shader/RShader.h"

namespace At0::Ray
{
	Layer::Layer(Scene& scene) : m_Scene(&scene) {}

	GraphicsPipeline::Builder Layer::PipelineBuilder()
	{
		return GraphicsPipeline::Builder();
	}

	Ref<Shader> Layer::LoadShaderFromSourceFile(
		const std::vector<std::string>& shaders, const std::vector<std::string>& reflections)
	{
		return Shader::AcquireSourceFile(shaders, reflections);
	}

	Ref<Shader> Layer::LoadShaderFromCompiledFile(
		const std::vector<std::string>& shaders, const std::vector<std::string>& reflections)
	{
		return Shader::AcquireCompiledFile(shaders, reflections);
	}

	Material::Builder Layer::MaterialBuilder(Ref<GraphicsPipeline> pipeline)
	{
		return Material::Builder(pipeline);
	}
}  // namespace At0::Ray