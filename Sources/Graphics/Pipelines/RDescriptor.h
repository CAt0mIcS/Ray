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
		enum class Frequency : uint8_t
		{
			PerScene = 0,
			PerObject = 1
		};

	public:
		DescriptorSet(VkDescriptorPool pool, VkDescriptorSetLayout descriptorLayout,
			Pipeline::BindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
			Frequency setNumber);
		DescriptorSet() = default;

		~DescriptorSet() = default;

		void CmdBind(const CommandBuffer& cmdBuff) const override;
		static void Update(const std::vector<VkWriteDescriptorSet>& descriptorWrites);
		Frequency GetFrequency() const { return m_Frequency; }

		operator const VkDescriptorSet&() const { return m_DescriptorSet; }

		DescriptorSet& operator=(DescriptorSet&& other) noexcept;
		DescriptorSet(DescriptorSet&& other) noexcept;

	private:
		VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;
		Frequency m_Frequency;

		Pipeline::BindPoint m_PipelineBindPoint;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
	};
}  // namespace At0::Ray
