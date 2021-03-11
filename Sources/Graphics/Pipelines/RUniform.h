#pragma once

#include "../../RBase.h"

#include "../Core/RBindable.h"
#include "RDescriptor.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class Pipeline;

	/**
	 * Describes everything that is neccessary to upload a uniform
	 */
	class RAY_EXPORT Uniform : public Bindable
	{
	public:
		Uniform(VkDescriptorSetLayout descSetLayout, VkDescriptorPool descSetPool,
			Pipeline::BindPoint bindPoint, VkPipelineLayout pipelineLayout, uint32_t bufferSize,
			uint32_t binding, uint32_t set = 0,
			VkDescriptorType descType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		Uniform(const Pipeline& pipeline, uint32_t bufferSize, uint32_t binding, uint32_t set = 0,
			VkDescriptorType descType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

		void CmdBind(const CommandBuffer& cmdBuff) const override;

		uint32_t GetGlobalBufferOffset() const { return m_GlobalBufferOffset; }

	private:
		void Setup(uint32_t bufferSize, uint32_t binding, VkDescriptorType descType);

	private:
		DescriptorSet m_DescriptorSet;
		uint32_t m_GlobalBufferOffset;
	};
}  // namespace At0::Ray
