#include "Rpch.h"
#include "RDynamicBuffer.h"

#include "Utils/RLogger.h"
#include "Utils/RAssert.h"


namespace At0::Ray
{
	DynamicBuffer::DynamicBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, const void* data)
		: Buffer(size, usage, properties, data), m_BufferUsage(std::move(usage)),
		  m_MemoryProperties(std::move(properties))
	{
		RAY_MEXPECTS((m_MemoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT),
			"[DynamicBuffer] Needs to have VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT");

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

	void DynamicBuffer::MapMemory(void** data, uint32_t offset)
	{
		m_Buffers[GetBufferID(offset)]->MapMemory(data);
	}

	void DynamicBuffer::UnmapMemory(uint32_t offset)
	{
		m_Buffers[GetBufferID(offset)]->UnmapMemory();
	}

	void DynamicBuffer::CopyBuffer(uint32_t srcOffset, uint32_t dstOffset, uint32_t size)
	{
		if (GetBufferID(srcOffset) == GetBufferID(dstOffset))
		{
			Buffer* buffer = m_Buffers[GetBufferID(srcOffset)];

			void* data;
			MapMemory(&data, srcOffset);

			memcpy((char*)data + GetOffset(dstOffset), (char*)data + GetOffset(srcOffset), size);
			UnmapMemory(srcOffset);

			if (!buffer->IsHostCoherent())
				buffer->FlushMemory();
		}
		else
		{
			uint32_t srcBufferID = GetBufferID(srcOffset);
			uint32_t dstBufferID = GetBufferID(dstOffset);

			void* srcData;
			m_Buffers[srcBufferID]->MapMemory(&srcData);
			void* dstData;
			m_Buffers[dstBufferID]->MapMemory(&dstData);

			memcpy(
				(char*)dstData + GetOffset(dstOffset), (char*)srcData + GetOffset(srcOffset), size);

			m_Buffers[srcBufferID]->UnmapMemory();
			m_Buffers[dstBufferID]->UnmapMemory();

			if (!m_Buffers[dstBufferID]->IsHostCoherent())
				m_Buffers[dstBufferID]->FlushMemory(
					/*size, GetOffset(dstOffset)*/);  // RAY_TODO: Only flush updated memory range
		}
	}

	void DynamicBuffer::Emplace(uint32_t allocSize, uint32_t alignment, uint32_t* offset)
	{
		// Resize the buffer if the requested data goes beyond buffer size
		uint32_t prevSize = m_Size;
		if (m_EmplaceLocation + allocSize >= GetTotalSize())
		{
			Reallocate(allocSize < s_LowestReallocationSize ?
							 s_LowestReallocationSize :
							 allocSize);	// Request more size than neccessary
		}

		*offset = m_EmplaceLocation;
		InternalEmplace(nullptr, Buffer::PadSizeToAlignment(allocSize, alignment));
	}

	void DynamicBuffer::Update(const void* data, uint32_t size, uint32_t offset)
	{
		if (!data)
			return;

		uint32_t bufferID = GetBufferID(offset);
		uint32_t localOffset = GetOffset(offset);

		void* mapped;
		m_Buffers[bufferID]->MapMemory(&mapped);
		memcpy((char*)mapped + localOffset, data, size);

		if (!m_Buffers[bufferID]->IsHostCoherent())
			m_Buffers[bufferID]->FlushMemory(size, localOffset);

		m_Buffers[bufferID]->UnmapMemory();
	}

	uint32_t DynamicBuffer::GetOffset(uint32_t globalOffset) const
	{
		uint32_t totalSizes = 0;
		for (uint32_t id = 0; id < m_Buffers.size(); ++id)
		{
			totalSizes += m_Buffers[id]->GetSize();
			if (totalSizes > globalOffset)
				return m_Buffers[id]->GetSize() - (totalSizes - globalOffset);
		}

		RAY_ASSERT(false,
			"[DynamicBuffer] Failed to find local buffer offset for global buffer offset {0}",
			globalOffset);
		return 0;
	}

	uint32_t DynamicBuffer::GetTotalSize() const
	{
		uint32_t totalSize = 0;
		for (Buffer* buff : m_Buffers)
			totalSize += buff->GetSize();

		return totalSize;
	}

	void DynamicBuffer::InternalEmplace(const void* data, uint32_t size)
	{
		Update(data, size, m_EmplaceLocation);
		m_EmplaceLocation += size;
	}

	void DynamicBuffer::Reallocate(uint32_t size)
	{
		Log::Info("[DynamicBuffer] Allocating new buffer {0}", m_Buffers.size() + 1);
		m_Buffers.emplace_back(new Buffer(size, m_BufferUsage, m_MemoryProperties));
	}

	uint32_t DynamicBuffer::GetBufferID(uint32_t offset) const
	{
		uint32_t totalSizes = 0;
		for (uint32_t id = 0; id < m_Buffers.size(); ++id)
		{
			totalSizes += m_Buffers[id]->GetSize();
			if (totalSizes > offset)
				return id;
		}

		RAY_ASSERT(false, "[DynamicBuffer] Failed to find buffer ID for offset {0}", offset);
		return 0;
	}
}  // namespace At0::Ray
