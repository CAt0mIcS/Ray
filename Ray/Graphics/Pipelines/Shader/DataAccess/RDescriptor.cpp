﻿#include "RDescriptor.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"

#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Buffers/Dynamic/RDynamicUniformBuffer.h"
#include "Graphics/Buffers/RBuffer.h"
#include "Graphics/Images/RImageView.h"

#include "RBufferUniform.h"
#include "RSampler2DUniform.h"
#include "Graphics/Images/RTexture.h"


namespace At0::Ray
{
	DescriptorSet::DescriptorSet(const Pipeline& pipeline, uint32_t setNumber)
		: DescriptorSet(pipeline.GetDescriptorPool(), pipeline.GetDescriptorSetLayout(setNumber),
			  pipeline.GetBindPoint(), pipeline.GetLayout(), setNumber)
	{
	}

	DescriptorSet::DescriptorSet(VkDescriptorPool pool, VkDescriptorSetLayout descriptorLayout,
		Pipeline::BindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout, uint32_t setNumber)
		: m_PipelineBindPoint(pipelineBindPoint), m_PipelineLayout(pipelineLayout),
		  m_SetNumber(setNumber), m_DescriptorPool(pool), m_DescriptorSetLayout(descriptorLayout)
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_DescriptorSetLayout;

		ThrowVulkanError(
			vkAllocateDescriptorSets(Graphics::Get().GetDevice(), &allocInfo, &m_DescriptorSet),
			"[DescriptorSet] Failed to allocate");
	}

	void DescriptorSet::CmdBind(const CommandBuffer& cmdBuff) const
	{
		RAY_DEBUG_FLAG(if (!m_UniformBound)
				Log::Warn("[DescriptorSet] No uniforms bound to descriptor set {0}", m_SetNumber));

		vkCmdBindDescriptorSets(cmdBuff, (VkPipelineBindPoint)m_PipelineBindPoint, m_PipelineLayout,
			m_SetNumber, 1, &m_DescriptorSet, 0, nullptr);
	}

	void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet>& descriptorWrites)
	{
		vkUpdateDescriptorSets(Graphics::Get().GetDevice(), (uint32_t)descriptorWrites.size(),
			descriptorWrites.data(), 0, nullptr);
	}

	void DescriptorSet::BindUniform(const BufferUniform& uniform)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniform.GetUniformBuffer().GetBuffer();
		bufferInfo.offset = uniform.GetOffset();
		bufferInfo.range = uniform.GetSize();

		VkWriteDescriptorSet descWrite{};
		descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWrite.dstSet = m_DescriptorSet;
		descWrite.dstBinding = uniform.GetBinding();
		descWrite.dstArrayElement = 0;
		descWrite.descriptorCount = 1;
		descWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descWrite.pBufferInfo = &bufferInfo;

		Update({ descWrite });

		RAY_DEBUG_FLAG(m_UniformBound = true);
	}

	void DescriptorSet::BindUniform(
		const Sampler2DUniform& uniform, Ref<Texture> texture, VkImageLayout imageLayout)
	{
		RAY_DEBUG_FLAG(m_Texture = texture.get());

		VkDescriptorImageInfo imageInfo{};
		imageInfo.sampler = texture->GetSampler();
		imageInfo.imageView = texture->GetImageView();
		imageInfo.imageLayout =
			imageLayout == VK_IMAGE_LAYOUT_MAX_ENUM ? texture->GetImageLayout() : imageLayout;

		VkWriteDescriptorSet descWrites{};
		descWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWrites.dstSet = m_DescriptorSet;
		descWrites.dstBinding = uniform.GetBinding();
		descWrites.dstArrayElement = 0;
		descWrites.descriptorCount = 1;
		descWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descWrites.pImageInfo = &imageInfo;

		Update({ descWrites });

		RAY_DEBUG_FLAG(m_UniformBound = true);
	}

	DescriptorSet& DescriptorSet::operator=(DescriptorSet&& other) noexcept
	{
		RAY_DEBUG_FLAG(m_UniformBound = other.m_UniformBound);
		RAY_DEBUG_FLAG(m_Texture = other.m_Texture);

		m_DescriptorSet = other.m_DescriptorSet;
		m_SetNumber = other.m_SetNumber;
		m_PipelineBindPoint = other.m_PipelineBindPoint;
		m_PipelineLayout = other.m_PipelineLayout;
		m_DescriptorPool = other.m_DescriptorPool;
		m_DescriptorSetLayout = other.m_DescriptorSetLayout;
		return *this;
	}
}  // namespace At0::Ray
