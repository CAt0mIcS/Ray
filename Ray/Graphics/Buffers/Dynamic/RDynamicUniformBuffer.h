#pragma once

#include "RDynamicBuffer.h"

#include <atomic>


namespace At0::Ray
{
	class UniformBuffer;
	class RenderContext;

	class RAY_EXPORT DynamicUniformBuffer : public DynamicBuffer
	{
	public:
		DynamicUniformBuffer(const RenderContext& context, VkDeviceSize size);
		~DynamicUniformBuffer();

		static void Reset();

		DynamicUniformBuffer(DynamicUniformBuffer&& other) noexcept;
		DynamicUniformBuffer& operator=(DynamicUniformBuffer&& other) noexcept;

	private:
		static Ref<UniformBuffer> s_UniformBuffer;
		static std::atomic<VkDeviceSize> s_NextOffset;
	};
}  // namespace At0::Ray
