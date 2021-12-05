#include "RSceneDescriptor.h"

#include "Utils/RException.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Pipelines/RPipeline.h"
#include "Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "RCamera.h"


namespace At0::Ray
{
	SceneDescriptor::SceneDescriptor()
	{  // Create per-scene descriptor set
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

		ThrowVulkanError(vkCreateDescriptorSetLayout(Graphics::Get().GetDevice(),
							 &descriptorSetLayoutCreateInfo, nullptr, &m_DescriptorSetLayout),
			"[Scene] Failed to create descriptor set layout for per scene data");

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.flags = 0;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		ThrowVulkanError(vkCreatePipelineLayout(Graphics::Get().GetDevice(),
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

		ThrowVulkanError(vkCreateDescriptorPool(Graphics::Get().GetDevice(),
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

	SceneDescriptor::~SceneDescriptor()
	{
		vkDestroyDescriptorPool(Graphics::Get().GetDevice(), m_DescriptorPool, nullptr);
		vkDestroyPipelineLayout(Graphics::Get().GetDevice(), m_PipelineLayout, nullptr);
		vkDestroyDescriptorSetLayout(Graphics::Get().GetDevice(), m_DescriptorSetLayout, nullptr);
	}

	void SceneDescriptor::UpdateUniform(const Camera& camera)
	{
		// We wait here to avoid some images having an old camera matrix while new ones already
		// have the new one (RAY_TODO: Synchronization so that this is not required)
		Graphics::Get().GetDevice().WaitIdle();
		m_PerSceneUniform->Update(camera.ShaderData);
	}

	void SceneDescriptor::CmdBind(const CommandBuffer& cmdBuff) const
	{
		m_PerSceneDescriptor->CmdBind(cmdBuff);
	}
}  // namespace At0::Ray
