#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"
#include "Graphics/Core/RBindable.h"

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
			VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
			Frequency setNumber);
		~DescriptorSet() = default;

		void CmdBind(const CommandBuffer& cmdBuff) const override;
		static void Update(const std::vector<VkWriteDescriptorSet>& descriptorWrites);
		Frequency GetFrequency() const { return m_Frequency; }

		operator const VkDescriptorSet&() const { return m_DescriptorSet; }

		DescriptorSet& operator=(DescriptorSet&& other) noexcept;
		DescriptorSet(DescriptorSet&& other) noexcept;

	private:
		VkDescriptorSet m_DescriptorSet;
		Frequency m_Frequency;

		VkPipelineBindPoint m_PipelineBindPoint;
		VkPipelineLayout m_PipelineLayout;
	};
}  // namespace At0::Ray
