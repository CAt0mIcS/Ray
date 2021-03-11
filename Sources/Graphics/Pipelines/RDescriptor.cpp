#include "Rpch.h"
#include "RDescriptor.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "RGraphicsPipeline.h"
#include "Graphics/Commands/RCommandBuffer.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	DescriptorSet::DescriptorSet(VkDescriptorPool pool, VkDescriptorSetLayout descriptorLayout,
		Pipeline::BindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout, uint32_t setNumber)
		: m_PipelineBindPoint(pipelineBindPoint), m_PipelineLayout(pipelineLayout),
		  m_SetNumber(setNumber)
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &descriptorLayout;

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
