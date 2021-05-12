#include "Rpch.h"
#include "RDescriptor.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "../RGraphicsPipeline.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Images/RTexture2D.h"
#include "RBufferUniform.h"
#include "RSampler2DUniform.h"

#include "Utils/RException.h"


namespace At0::Ray
{
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

		RAY_VK_THROW_FAILED(
			vkAllocateDescriptorSets(Graphics::Get().GetDevice(), &allocInfo, &m_DescriptorSet),
			"[DescriptorSet] Failed to allocate");
	}

	void DescriptorSet::CmdBind(const CommandBuffer& cmdBuff) const
	{
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
		bufferInfo.buffer = uniform.GetBuffer();
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
	}

	void DescriptorSet::BindUniform(const Sampler2DUniform& uniform)
	{
		VkDescriptorImageInfo imageInfo{};
		imageInfo.sampler = uniform.GetTexture()->GetSampler();
		imageInfo.imageView = uniform.GetTexture()->GetImageView();
		imageInfo.imageLayout = uniform.GetTexture()->GetImageLayout();

		VkWriteDescriptorSet descWrites{};
		descWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWrites.dstSet = m_DescriptorSet;
		descWrites.dstBinding = uniform.GetBinding();
		descWrites.dstArrayElement = 0;
		descWrites.descriptorCount = 1;
		descWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descWrites.pImageInfo = &imageInfo;

		Update({ descWrites });
	}

	DescriptorSet& DescriptorSet::operator=(DescriptorSet&& other) noexcept
	{
		m_DescriptorSet = other.m_DescriptorSet;
		m_SetNumber = other.m_SetNumber;
		m_PipelineBindPoint = other.m_PipelineBindPoint;
		m_PipelineLayout = other.m_PipelineLayout;
		return *this;
	}

	DescriptorSet::DescriptorSet(DescriptorSet&& other) noexcept
		: m_DescriptorSet(other.m_DescriptorSet), m_SetNumber(other.m_SetNumber),
		  m_PipelineBindPoint(other.m_PipelineBindPoint), m_PipelineLayout(other.m_PipelineLayout)
	{
	}
}  // namespace At0::Ray
