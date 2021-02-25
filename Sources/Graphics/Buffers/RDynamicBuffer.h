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

		/**
		 * Resizes the buffer to the new size.
		 * Discards any data if the newSize is less than the current size.
		 * When enlargening the buffer, the new data is undefined.
		 */
		void Resize(VkDeviceSize newSize);

		/**
		 * Appends the data at the end of the buffer.
		 * Resizes the buffer if the capacity is not enough
		 * @param data The data to place at the end of the uniform buffer
		 */
		template<typename T>
		void Emplace(T&& data)
		{
			InternalEmplace(&data, sizeof(T));
		}

		/**
		 * Updates a specific part of the buffer
		 * @param data The data to add to the buffer
		 * @param size The size of the data
		 * @param offset The offset in the buffer
		 */
		void Update(const void* data, uint32_t size, uint32_t offset);

	private:
		void InternalEmplace(void* data, uint32_t size);

	private:
		VkBufferUsageFlags m_BufferUsage{};
		VkMemoryPropertyFlags m_MemoryProperties{};

		uint32_t m_Capacity = 0;
	};
}  // namespace At0::Ray
