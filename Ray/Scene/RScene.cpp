#include "Rpch.h"

#include "REntity.h"
#include "RScene.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "Ray/Utils/RException.h"
#include "Ray/Utils/RLogger.h"

#include "Components/RMeshRenderer.h"
#include "Components/RTextRenderer.h"
#include "Components/RSkybox.h"
#include "Components/RTransform.h"
#include "Core/RRendererLoader.h"

#include "Events/REventListener.h"

// RAY_TEMPORARY
#include "Core/RTime.h"

#define RAY_MULTITHREADED_TRANSFORM_CALCULATIONS 1
#define RAY_MULTITHREADED_MESHRENDERER_UPDATES 1


namespace At0::Ray
{
	Scope<Scene> Scene::s_CurrentScene = nullptr;

	Scene& Scene::Get()
	{
		RAY_MEXPECTS(s_CurrentScene, "[Scene] Current Scene was not set");
		return *s_CurrentScene;
	}

	void Scene::Destroy() { s_CurrentScene = nullptr; }

	Scene::~Scene()
	{
		m_Registry.clear();
		RendererAPI::DestroyDescriptorPool(Graphics::Get().GetDevice(), m_DescriptorPool);
		RendererAPI::DestroyPipelineLayout(Graphics::Get().GetDevice(), m_PipelineLayout);
		RendererAPI::DestroyDescriptorSetLayout(Graphics::Get().GetDevice(), m_DescriptorSetLayout);
	}

	Entity Scene::CreateEntity()
	{
		Entity entity(m_Registry.create(), &m_Registry);

		// RAY_TODO: Should maybe be moved to client
		entity.Emplace<Transform>();

		// Dispatch entity created event to listeners
		EntityCreatedEvent e(entity);
		for (EventListener<EntityCreatedEvent>* listener :
			EventDispatcher<EntityCreatedEvent>::Get())
		{
			listener->OnEvent(e);
		}

		return entity;
	}

	void Scene::DestroyEntity(Entity entity) { m_Registry.destroy((entt::entity)entity); }

	void Scene::Update(Delta dt)
	{
		m_Camera->Update(dt);

		auto tformView = m_Registry.view<Transform>();

		// RAY_TODO: Global define to enable/disable profiling
		Time tStart = Time::Now();

#if RAY_MULTITHREADED_TRANSFORM_CALCULATIONS
		m_ThreadPool.SubmitLoop(0u, (uint32_t)tformView.size(), [this, &tformView](uint32_t i) {
			Entity{ tformView[i] }.Get<Transform>().UpdateMatrix();
		});
		m_ThreadPool.WaitForTasks();
#else
		tformView.each([](Transform& tform) { tform.UpdateMatrix(); });
#endif

		CLog::Trace("[Scene] Transformation recalculations took {0}us",
			(Time::Now() - tStart).AsMicroseconds());

		tStart = Time::Now();

#if RAY_MULTITHREADED_MESHRENDERER_UPDATES
		auto meshRendererView = m_Registry.view<MeshRenderer>();
		m_ThreadPool.SubmitLoop(
			0u, (uint32_t)meshRendererView.size(), [&meshRendererView](uint32_t i) {
				Entity{ meshRendererView[i] }.Get<MeshRenderer>().Update();
			});
		m_ThreadPool.WaitForTasks();

#else
		m_Registry.view<MeshRenderer>().each([](MeshRenderer& mesh) { mesh.Update(); });
#endif

		m_Registry.view<TextRenderer>().each([](TextRenderer& text) { text.Update(); });

		CLog::Trace(
			"[Scene] MeshRenderer updates took {0}us", (Time::Now() - tStart).AsMicroseconds());
	}

	void Scene::SetCamera(Scope<Camera> cam) { m_Camera = std::move(cam); }

	const Camera& Scene::GetCamera() const
	{
		RAY_MEXPECTS(m_Camera, "[Scene] Camera not created");
		return *m_Camera;
	}

	void Scene::CmdBind(const CommandBuffer& cmdBuff) const
	{
		m_PerSceneDescriptor->CmdBind(cmdBuff);
	}

	Scene::Scene(Scope<Camera> camera)
		: EventListener<CameraChangedEvent>(*camera), m_Camera(std::move(camera))
	{
		Graphics::Create();

		s_CurrentScene = Scope<Scene>(this);
		SetupPerSceneUniform();
	}

	void Scene::SetupPerSceneUniform()
	{
		// Create per-scene descriptor set
		RrDescriptorSetLayoutBinding perSceneBinding{};
		perSceneBinding.binding = 0;
		perSceneBinding.descriptorType = RrDescriptorTypeUniformBuffer;
		perSceneBinding.descriptorCount = 1;
		perSceneBinding.stageFlags = RrShaderStageVertex;
		perSceneBinding.pImmutableSamplers = nullptr;

		RrDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
		descriptorSetLayoutCreateInfo.bindingCount = 1;
		descriptorSetLayoutCreateInfo.pBindings = &perSceneBinding;

		ThrowRenderError(RendererAPI::CreateDescriptorSetLayout(Graphics::Get().GetDevice(),
							 &descriptorSetLayoutCreateInfo, &m_DescriptorSetLayout),
			"[Scene] Failed to create descriptor set layout for per scene data");

		RrPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		ThrowRenderError(RendererAPI::CreatePipelineLayout(Graphics::Get().GetDevice(),
							 &pipelineLayoutCreateInfo, &m_PipelineLayout),
			"[Scene] Failed to create pipeline layout for per scene data");

		RrDescriptorPoolSize poolSize{};
		poolSize.type = RrDescriptorTypeUniformBuffer;
		poolSize.descriptorCount = 1;

		RrDescriptorPoolCreateInfo descriptorPoolCreateInfo{};
		descriptorPoolCreateInfo.maxSets = 1;
		descriptorPoolCreateInfo.poolSizeCount = 1;
		descriptorPoolCreateInfo.pPoolSizes = &poolSize;

		ThrowRenderError(RendererAPI::CreateDescriptorPool(Graphics::Get().GetDevice(),
							 &descriptorPoolCreateInfo, &m_DescriptorPool),
			"[Scene] Failed to create descriptor pool");

		m_PerSceneDescriptor = MakeScope<DescriptorSet>(m_DescriptorPool, m_DescriptorSetLayout,
			Pipeline::BindPoint::Graphics, m_PipelineLayout,
			0);	 // set number 0 reserved for per scene data

		// Predefined layout for per scene uniform
		std::unordered_map<std::string, uint32_t> uniformInBlockOffsets;
		uniformInBlockOffsets["View"] = offsetof(PerSceneData, View);
		uniformInBlockOffsets["Proj"] = offsetof(PerSceneData, Projection);
		uniformInBlockOffsets["ViewPos"] = offsetof(PerSceneData, ViewPos);
		uniformInBlockOffsets["LightPos"] = offsetof(PerSceneData, LightPos);
		m_PerSceneUniform = MakeScope<BufferUniform>(
			"PerSceneData", 0, (uint32_t)sizeof(PerSceneData), std::move(uniformInBlockOffsets));

		m_PerSceneDescriptor->BindUniform(*m_PerSceneUniform);
	}

	void Scene::UpdateUniform()
	{
		// We wait here to avoid some images having an old camera matrix while new ones already
		// have the new one (RAY_TODO: Synchronization so that this is not required)
		Graphics::Get().GetDevice().WaitIdle();
		m_PerSceneUniform->Update(m_Camera->ShaderData);
	}

}  // namespace At0::Ray
