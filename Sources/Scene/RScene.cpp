#include "Rpch.h"

#include "REntity.h"
#include "RScene.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "Utils/RException.h"
#include "Utils/RLogger.h"

#include "Components/RMesh.h"
#include "Components/RSkybox.h"
#include "Components/RTransform.h"

#include "Events/REventListener.h"


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
		vkDestroyDescriptorPool(Graphics::Get().GetDevice(), m_DescriptorPool, nullptr);
		vkDestroyPipelineLayout(Graphics::Get().GetDevice(), m_PipelineLayout, nullptr);
		vkDestroyDescriptorSetLayout(Graphics::Get().GetDevice(), m_DescriptorSetLayout, nullptr);
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

		// RAY_MESH_REWORK:
		// m_Registry.view<Mesh>().each([&dt](Mesh& mesh) { mesh.Update(dt); });
		m_Registry.view<Skybox>().each([&dt](Skybox& skybox) { skybox.Update(dt); });
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
		s_CurrentScene = Scope<Scene>(this);
		SetupPerSceneUniform();
	}

	void Scene::SetupPerSceneUniform()
	{
		// Create per-scene descriptor set
		VkDescriptorSetLayoutBinding perSceneBinding{};
		perSceneBinding.binding = 0;
		perSceneBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		perSceneBinding.descriptorCount = 1;
		perSceneBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		perSceneBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = 1;
		descriptorSetLayoutCreateInfo.pBindings = &perSceneBinding;

		RAY_VK_THROW_FAILED(vkCreateDescriptorSetLayout(Graphics::Get().GetDevice(),
								&descriptorSetLayoutCreateInfo, nullptr, &m_DescriptorSetLayout),
			"[Scene] Failed to create descriptor set layout for per scene data");

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.flags = 0;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		RAY_VK_THROW_FAILED(vkCreatePipelineLayout(Graphics::Get().GetDevice(),
								&pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout),
			"[Scene] Failed to create pipeline layout for per scene data");

		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = 1;

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.maxSets = 1;
		descriptorPoolCreateInfo.poolSizeCount = 1;
		descriptorPoolCreateInfo.pPoolSizes = &poolSize;

		RAY_VK_THROW_FAILED(vkCreateDescriptorPool(Graphics::Get().GetDevice(),
								&descriptorPoolCreateInfo, nullptr, &m_DescriptorPool),
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
		// We wait here to avoid some images having an old camera matrix while new ones already have
		// the new one (RAY_TODO: Synchronization so that this is not required)
		Graphics::Get().GetDevice().WaitIdle();
		m_PerSceneUniform->Update(m_Camera->ShaderData);
	}

}  // namespace At0::Ray
