#include "Rpch.h"
#include "RDescriptor.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "RGraphicsPipeline.h"
#include "Graphics/Commands/RCommandBuffer.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	DescriptorSet::DescriptorSet(const GraphicsPipeline& pipeline) : m_Pipeline(pipeline)
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_Pipeline.GetDescriptorPool();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_Pipeline.GetDescriptorSetLayout();

		RAY_VK_THROW_FAILED(
			vkAllocateDescriptorSets(Graphics::Get().GetDevice(), &allocInfo, &m_DescriptorSet),
			"[DescriptorSet] Failed to allocate.");
	}

	void DescriptorSet::CmdBind(const CommandBuffer& cmdBuff) const
	{
		vkCmdBindDescriptorSets(cmdBuff, m_Pipeline.GetBindPoint(), m_Pipeline.GetLayout(), 0, 1,
			&m_DescriptorSet, 0, nullptr);
	}

	void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet>& descriptorWrites)
	{
		vkUpdateDescriptorSets(Graphics::Get().GetDevice(), (uint32_t)descriptorWrites.size(),
			descriptorWrites.data(), 0, nullptr);
	}
}  // namespace At0::Ray
