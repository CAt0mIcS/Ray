#pragma once

#include "RBuffer.h"


namespace At0::Ray
{
	/**
	 * Represents a resizable vulkan buffer. Will only work if it is host visible.
	 */
	class RAY_EXPORT DynamicBuffer : public Buffer
	{
	public:
		DynamicBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
			const void* data = nullptr);
		virtual ~DynamicBuffer();

		void Emplace(uint32_t allocSize, uint32_t alignment, uint32_t* offset, uint32_t* bufferID);
		void Update(const void* data, uint32_t size, uint32_t offset, uint32_t bufferID);

		const Buffer& GetBuffer(uint32_t bufferID) const { return *m_Buffers[bufferID]; }

	private:
		void InternalEmplace(const void* data, uint32_t size);
		void Reallocate(uint32_t size);

	private:
		VkBufferUsageFlags m_BufferUsage{};
		VkMemoryPropertyFlags m_MemoryProperties{};

		uint32_t m_EmplaceLocation = 0;

		// Allocate new buffers if the last one in the list is full
		std::vector<Buffer*> m_Buffers{};

		inline static constexpr uint32_t s_LowestReallocationSize = 1048576;
	};
}  // namespace At0::Ray
