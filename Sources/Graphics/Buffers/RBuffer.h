#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT Buffer : NonCopyable
	{
	public:
		Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
			const void* data = nullptr);
		Buffer(VkDeviceSize size);
		virtual ~Buffer();

		void MapMemory(void** data) const;
		void UnmapMemory() const;
		void Update(const void* data, uint32_t size, uint32_t offset);
		void FlushMemory();

		static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		operator const VkBuffer&() const { return m_Buffer; }

	protected:
		void Destroy();

	protected:
		VkBuffer m_Buffer = VK_NULL_HANDLE;
		VkDeviceMemory m_BufferMemory = VK_NULL_HANDLE;

		VkDeviceSize m_Size = 0;
		bool m_IsHostCoherent = false;
	};
}  // namespace At0::Ray
