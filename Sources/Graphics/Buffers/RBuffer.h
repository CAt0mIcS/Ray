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
		Buffer() = default;
		virtual ~Buffer();

		void MapMemory(void** data) const;
		void UnmapMemory() const;
		void FlushMemory() const;


		static void MapMemory(void** data, VkDeviceMemory memory, VkDeviceSize size);
		static void UnmapMemory(VkDeviceMemory memory);
		static void FlushMemory(VkDeviceMemory bufferMemory, VkDeviceSize size);

		static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		static uint32_t PadSizeToAlignment(uint32_t originalSize, uint32_t alignment);
		static void BindBufferToMemory(VkBuffer buffer, VkDeviceMemory memory);

		const VkBuffer& GetBuffer() const { return m_Buffer; }
		VkDeviceSize GetSize() const { return m_Size; }
		const VkDeviceMemory& GetMemory() const { return m_BufferMemory; }
		operator const VkBuffer&() const { return m_Buffer; }

		Buffer& operator=(Buffer&& other);
		Buffer(Buffer&& other);

	protected:
		void Destroy();

	protected:
		VkBuffer m_Buffer = VK_NULL_HANDLE;
		VkDeviceMemory m_BufferMemory = VK_NULL_HANDLE;

		VkDeviceSize m_Size = 0;
		bool m_IsHostCoherent = false;
	};
}  // namespace At0::Ray
