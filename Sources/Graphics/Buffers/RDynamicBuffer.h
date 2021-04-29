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

		/**
		 * When mappping the dynamic buffer the offset of the data is required to be able to choose
		 * the correct buffer
		 */
		void MapMemory(void** data, uint32_t offset);

		/**
		 * When unmapping the dynamic buffer the offset is required to be able to choose
		 * the correct buffer. If the buffer at offset is not mapped, behaviour is undefined.
		 */
		void UnmapMemory(uint32_t offset);

		/**
		 * Copies a range of data to a new location in the buffer
		 * @param srcOffset The global data offset of the source data
		 * @param dstOffset The global data offset of the destination memory location
		 * @param size Size of the data to copy
		 */
		void CopyBuffer(uint32_t srcOffset, uint32_t dstOffset, uint32_t size);

		void Emplace(uint32_t allocSize, uint32_t alignment, uint32_t* offset);
		void Update(const void* data, uint32_t size, uint32_t offset);
		uint32_t GetOffset(uint32_t globalOffset) const;
		uint32_t GetTotalSize() const;

		const Buffer& GetBuffer(uint32_t offset) const { return *m_Buffers[GetBufferID(offset)]; }

	private:
		void InternalEmplace(const void* data, uint32_t size);
		void Reallocate(uint32_t size);
		uint32_t GetBufferID(uint32_t offset) const;

	private:
		VkBufferUsageFlags m_BufferUsage{};
		VkMemoryPropertyFlags m_MemoryProperties{};

		uint32_t m_EmplaceLocation = 0;

		// Allocate new buffers if the last one in the list is full
		std::vector<Buffer*> m_Buffers{};

		static constexpr uint32_t s_LowestReallocationSize = 1048576;
	};
}  // namespace At0::Ray
