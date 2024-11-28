#pragma once

#include "../RBase.h"
#include "../Core/RTime.h"
#include "../Graphics/Pipelines/RGraphicsPipeline.h"
#include "../Graphics/Pipelines/Shader/RShaderTypes.h"
#include "../Shading/RMaterial.h"


namespace At0::Ray
{
	class Scene;
	class Shader;
	class ResourceManager;
	class Window;
	class Engine;

	class RAY_EXPORT Layer
	{
	public:
		Layer(Scene& scene);
		virtual ~Layer() = default;

		virtual void Update(Delta dt) {}

		Layer(const Layer&) = delete;
		Layer& operator=(const Layer&) = delete;

		Layer(Layer&&) = default;
		Layer& operator=(Layer&&) = default;

		const Scene& GetScene() const { return *m_Scene; }
		Scene& GetScene() { return *m_Scene; }

		const Window& GetWindow() const;
		Window& GetWindow() { return (Window&)std::as_const(*this).GetWindow(); }

		const Engine& GetEngine() const;

		const ResourceManager& GetResourceManager() const;
		ResourceManager& GetResourceManager()
		{
			return (ResourceManager&)std::as_const(*this).GetResourceManager();
		}

	protected:
		GraphicsPipeline::Builder PipelineBuilder();
		Ref<Shader> LoadShaderFromSourceFile(const std::vector<std::string>& shaders,
			const std::vector<std::string>& reflections = {});
		Ref<Shader> LoadShaderFromCompiledFile(const std::vector<std::string>& shaders,
			const std::vector<std::string>& reflections = {});
		Ref<Shader> LoadShaderFromString(const std::vector<std::string>& shaders,
			const std::vector<ShaderStage>& stageOrder,
			const std::vector<std::string>& reflections = {});

		Material::Builder MaterialBuilder(Ref<GraphicsPipeline> pipeline);


	private:
		Scene* m_Scene;
	};
}  // namespace At0::Ray