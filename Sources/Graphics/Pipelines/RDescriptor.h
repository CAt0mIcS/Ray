#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"
#include "Graphics/Core/RBindable.h"
#include "RPipeline.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class DescriptorSet : public Bindable, NonCopyable
	{
	public:
	public:
		DescriptorSet(VkDescriptorPool pool, VkDescriptorSetLayout descriptorLayout,
			Pipeline::BindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
			uint32_t setNumber);

		~DescriptorSet() = default;

		void CmdBind(const CommandBuffer& cmdBuff) const override;
		static void Update(const std::vector<VkWriteDescriptorSet>& descriptorWrites);
		uint32_t GetSetNumber() const { return m_SetNumber; }

		operator const VkDescriptorSet&() const { return m_DescriptorSet; }

		DescriptorSet& operator=(DescriptorSet&& other) noexcept;
		DescriptorSet(DescriptorSet&& other) noexcept;

	private:
		VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;
		uint32_t m_SetNumber;

		Pipeline::BindPoint m_PipelineBindPoint;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
	};
}  // namespace At0::Ray
