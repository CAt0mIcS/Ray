#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"
#include "Graphics/Core/RBindable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class GraphicsPipeline;

	class DescriptorSet : public Bindable, NonCopyable
	{
	public:
		enum class Frequency : uint8_t
		{
			PerObject = 0,
			PerScene = 1
		};

	public:
		DescriptorSet(const GraphicsPipeline& pipeline, Frequency setNumber);
		~DescriptorSet() = default;

		void CmdBind(const CommandBuffer& cmdBuff) const override;
		static void Update(const std::vector<VkWriteDescriptorSet>& descriptorWrites);

		operator const VkDescriptorSet&() const { return m_DescriptorSet; }

	private:
		VkDescriptorSet m_DescriptorSet;
		const GraphicsPipeline& m_Pipeline;
	};
}  // namespace At0::Ray
