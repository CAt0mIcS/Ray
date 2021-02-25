#include "Rpch.h"
#include "RDynamicBuffer.h"

#include "Utils/RAssert.h"


namespace At0::Ray
{
	DynamicBuffer::DynamicBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, const void* data)
		: Buffer(size, usage, properties, data), m_Capacity(size), m_BufferUsage(std::move(usage)),
		  m_MemoryProperties(std::move(properties))
	{
		RAY_MEXPECTS((m_MemoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
						 (m_MemoryProperties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
			"[DynamicBuffer] Needs to be visible to the host and host coherent.");
	}

	void DynamicBuffer::Resize(VkDeviceSize newSize)
	{
		std::vector<char> prevData(m_Size);

		// Store the previous data in the vector
		void* data;
		MapMemory(&data);
		memcpy(prevData.data(), data, m_Size);
		UnmapMemory();

		// Destroy the buffer and buffer memory
		Destroy();

		CreateBuffer(newSize, m_BufferUsage, m_MemoryProperties, m_Buffer, m_BufferMemory);
		m_Size = newSize;

		// Copy the cached data back to the new buffer. Make sure not to access invalid memory
		MapMemory(&data);
		memcpy(&data, prevData.data(), std::min(prevData.size(), m_Size));
		UnmapMemory();

		m_Capacity = m_Size;
	}

	void DynamicBuffer::Update(const void* data, uint32_t size, uint32_t offset)
	{
		if (!data)
			return;

		if (offset + size > m_Size)
			Resize(offset + size);

		void* mapped;
		MapMemory(&mapped);
		memcpy((char*)mapped + offset, data, size);

		if (!m_IsHostCoherent)
			FlushMemory();

		UnmapMemory();
	}

	void DynamicBuffer::InternalEmplace(void* data, uint32_t size)
	{
		// Resize the buffer if the requested data goes beyond buffer size
		uint32_t prevSize = m_Size;
		if (m_Size + size > m_Capacity)
			Resize(m_Size + size);

		void* mapped;
		MapMemory(&mapped);
		memcpy((char*)mapped + prevSize, data, size);
		UnmapMemory();
	}
}  // namespace At0::Ray
