#include "Rpch.h"
#include "RDynamicBuffer.h"

#include "Utils/RAssert.h"


namespace At0::Ray
{
	DynamicBuffer::DynamicBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, const void* data)
		: Buffer(size, usage, properties, data), m_BufferUsage(std::move(usage)),
		  m_MemoryProperties(std::move(properties))
	{
		RAY_MEXPECTS((m_MemoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
						 (m_MemoryProperties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
			"[DynamicBuffer] Needs to be visible to the host and host coherent.");

		m_Buffers.emplace_back((Buffer*)this);
	}

	DynamicBuffer::~DynamicBuffer()
	{
		// Make sure not to delete the first element as it's this buffer
		for (uint32_t i = 1; i < m_Buffers.size(); ++i)
		{
			delete m_Buffers[i];
		}
	}


	void DynamicBuffer::Emplace(
		uint32_t allocSize, uint32_t alignment, uint32_t* offset, uint32_t* bufferID)
	{
		// Resize the buffer if the requested data goes beyond buffer size
		uint32_t prevSize = m_Size;
		if (m_EmplaceLocation + allocSize >= m_Size)
		{
			Reallocate(allocSize < s_LowestReallocationSize ?
						   s_LowestReallocationSize :
						   allocSize);	// Request more size than neccessary
		}

		*offset = m_EmplaceLocation;
		InternalEmplace(nullptr, Buffer::PadSizeToAlignment(allocSize, alignment));
		*bufferID = m_Buffers.size() - 1;
	}

	void DynamicBuffer::Update(const void* data, uint32_t size, uint32_t offset, uint32_t bufferID)
	{
		if (!data)
			return;

		RAY_MEXPECTS(offset + size <= m_Size,
			"[DynamicBuffer] Does not have enough storage to hold the data");

		void* mapped;
		MapMemory(&mapped, m_Buffers[bufferID]->GetMemory(), m_Buffers[bufferID]->GetSize());
		memcpy((char*)mapped + offset, data, size);

		if (!m_IsHostCoherent)
			FlushMemory();

		UnmapMemory(m_Buffers[bufferID]->GetMemory());
	}

	void DynamicBuffer::InternalEmplace(const void* data, uint32_t size)
	{
		Update(data, size, m_EmplaceLocation, m_Buffers.size() - 1);
		m_EmplaceLocation += size;
	}

	void DynamicBuffer::Reallocate(uint32_t size)
	{
		m_Buffers.emplace_back(new Buffer(size, m_BufferUsage, m_MemoryProperties));
		m_EmplaceLocation = 0;
	}
}  // namespace At0::Ray
