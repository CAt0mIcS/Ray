#pragma once

#include "../../../RBase.h"
#include "../../../Utils/RNonCopyable.h"
#include "../../Core/RBindable.h"
#include "../RPipeline.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class BufferUniform;
	class Sampler2DUniform;

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

		void BindUniform(const BufferUniform& uniform);
		void BindUniform(const Sampler2DUniform& uniform);

		operator const VkDescriptorSet&() const { return m_DescriptorSet; }
		DescriptorSet& operator=(DescriptorSet&& other) noexcept;
		DescriptorSet(DescriptorSet&& other) noexcept;

	private:
		VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;
		VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
		VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
		uint32_t m_SetNumber;

		Pipeline::BindPoint m_PipelineBindPoint;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;

#ifndef NDEBUG
		bool m_UniformBound = false;
#endif
	};
}  // namespace At0::Ray
