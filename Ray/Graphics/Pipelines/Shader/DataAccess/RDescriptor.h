﻿#pragma once

#include "../../../../RBase.h"
#include "../../../../Utils/RNonCopyable.h"
#include "../../RPipeline.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class BufferUniform;
	class Sampler2DUniform;
	class Texture2D;
	RAY_DEBUG_FLAG(class Texture2D);

	class DescriptorSet : NonCopyable
	{
	public:
	public:
		DescriptorSet(VkDescriptorPool pool, VkDescriptorSetLayout descriptorLayout,
			Pipeline::BindPoint pipelineBindPoint, VkPipelineLayout pipelineLayout,
			uint32_t setNumber);

		~DescriptorSet() = default;

		void CmdBind(const CommandBuffer& cmdBuff) const;
		static void Update(const std::vector<VkWriteDescriptorSet>& descriptorWrites);
		uint32_t GetSetNumber() const { return m_SetNumber; }

		void BindUniform(const BufferUniform& uniform);
		void BindUniform(const Sampler2DUniform& uniform, Ref<Texture2D> texture);

		operator const VkDescriptorSet&() const { return m_DescriptorSet; }
		DescriptorSet& operator=(DescriptorSet&& other) noexcept;
		DescriptorSet(DescriptorSet&& other) noexcept { *this = std::move(other); }

	private:
		VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;
		VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
		VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
		uint32_t m_SetNumber;

		Pipeline::BindPoint m_PipelineBindPoint;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;

		RAY_DEBUG_FLAG(bool m_UniformBound = false);
		RAY_DEBUG_FLAG(Texture2D* m_Texture);
	};
}  // namespace At0::Ray