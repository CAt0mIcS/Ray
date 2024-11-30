#include "RLayer.h"

#include "Core/REngine.h"
#include "Scene/RScene.h"
#include "Devices/RWindow.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Pipelines/Shader/RShader.h"

namespace At0::Ray
{
	Layer::Layer(Scene& scene, Ref<Window> window) : m_Scene(&scene), m_Window(std::move(window)) {}

	const Engine& Layer::GetEngine() const
	{
		return GetScene().GetEngine();
	}

	const ResourceManager& Layer::GetResourceManager() const
	{
		return GetEngine().GetResourceManager();
	}

	GraphicsPipeline::Builder Layer::PipelineBuilder()
	{
		auto& graphics = GetWindow()->GetRenderContext().graphics;
		return GraphicsPipeline::Builder(graphics.GetRenderPass(), graphics.GetPipelineCache());
	}

	Ref<Shader> Layer::LoadShaderFromSourceFile(
		const std::vector<std::string>& shaders, const std::vector<std::string>& reflections)
	{
		auto window = m_Window.lock();
		if (!window)
			return nullptr;

		std::string tag = std::accumulate(shaders.begin(), shaders.end(), std::string{});
		if (auto stored = GetResourceManager().Get<Shader>(tag); stored)
			return stored;

		auto shader = Shader::FromSourceFile(window->GetRenderContext(), shaders, reflections);
		return GetResourceManager().Emplace<Shader>(tag, std::move(shader));
	}

	Ref<Shader> Layer::LoadShaderFromCompiledFile(
		const std::vector<std::string>& shaders, const std::vector<std::string>& reflections)
	{
		auto window = m_Window.lock();
		if (!window)
			return nullptr;

		std::string tag = std::accumulate(shaders.begin(), shaders.end(), std::string{});
		if (auto stored = GetResourceManager().Get<Shader>(tag); stored)
			return stored;

		auto shader = Shader::FromCompiledFile(window->GetRenderContext(), shaders, reflections);
		return GetResourceManager().Emplace<Shader>(tag, std::move(shader));
	}

	Ref<Shader> Layer::LoadShaderFromString(const std::vector<std::string>& shaders,
		const std::vector<ShaderStage>& stageOrder, const std::vector<std::string>& reflections)
	{
		auto window = m_Window.lock();
		if (!window)
			return nullptr;

		return Shader::FromSourceString(
			window->GetRenderContext(), shaders, stageOrder, reflections);
	}

	Material::Builder Layer::MaterialBuilder(Ref<GraphicsPipeline> pipeline)
	{
		return Material::Builder(pipeline);
	}
}  // namespace At0::Ray