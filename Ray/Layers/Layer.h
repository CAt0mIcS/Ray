#pragma once

#include "../RBase.h"
#include "../Graphics/Pipelines/RGraphicsPipeline.h"
#include "../Shading/RMaterial.h"


namespace At0::Ray
{
	class Scene;
	class Shader;

	class RAY_EXPORT Layer
	{
	public:
		Layer(Scene& scene);
		virtual ~Layer() = default;

		Layer(const Layer&) = delete;
		Layer& operator=(const Layer&) = delete;

		Layer(Layer&&) = default;
		Layer& operator=(Layer&&) = default;

		const Scene& GetScene() const { return *m_Scene; }
		Scene& GetScene() { return *m_Scene; }

	protected:
		GraphicsPipeline::Builder PipelineBuilder();
		Ref<Shader> LoadShaderFromSourceFile(const std::vector<std::string>& shaders,
			const std::vector<std::string>& reflections = {});
		Ref<Shader> LoadShaderFromCompiledFile(const std::vector<std::string>& shaders,
			const std::vector<std::string>& reflections = {});

		Material::Builder MaterialBuilder(Ref<GraphicsPipeline> pipeline);


	private:
		Scene* m_Scene;
	};
}  // namespace At0::Ray