#include "Rpch.h"
#include "RDescriptor.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "RGraphicsPipeline.h"
#include "Graphics/Commands/RCommandBuffer.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	DescriptorSet::DescriptorSet(const GraphicsPipeline& pipeline, Frequency setNumber)
		: m_Pipeline(&pipeline), m_Frequency(setNumber)
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_Pipeline->GetDescriptorPool();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_Pipeline->GetDescriptorSetLayout()[(size_t)m_Frequency];

		RAY_VK_THROW_FAILED(
			vkAllocateDescriptorSets(Graphics::Get().GetDevice(), &allocInfo, &m_DescriptorSet),
			"[DescriptorSet] Failed to allocate.");
	}

	void DescriptorSet::CmdBind(const CommandBuffer& cmdBuff) const
	{
		vkCmdBindDescriptorSets(cmdBuff, m_Pipeline->GetBindPoint(), m_Pipeline->GetLayout(),
			(uint32_t)m_Frequency, 1, &m_DescriptorSet, 0, nullptr);
	}

	void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet>& descriptorWrites)
	{
		vkUpdateDescriptorSets(Graphics::Get().GetDevice(), (uint32_t)descriptorWrites.size(),
			descriptorWrites.data(), 0, nullptr);
	}

	DescriptorSet& DescriptorSet::operator=(DescriptorSet&& other) noexcept
	{
		m_DescriptorSet = other.m_DescriptorSet;
		m_Pipeline = other.m_Pipeline;
		m_Frequency = other.m_Frequency;
		return *this;
	}

	DescriptorSet::DescriptorSet(DescriptorSet&& other) noexcept
		: m_DescriptorSet(other.m_DescriptorSet), m_Pipeline(other.m_Pipeline),
		  m_Frequency(other.m_Frequency)
	{
	}
}  // namespace At0::Ray
