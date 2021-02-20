#include "Rpch.h"
#include "RIndexBuffer.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"

#include "Utils/RAssert.h"


namespace At0::Ray
{
	IndexBuffer::IndexBuffer(const std::vector<Type>& indices)
		: Buffer(sizeof(IndexBuffer::Type) * indices.size(),
			  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		  m_NumIndices((uint32_t)indices.size())
	{
		// Create staging buffer
		Buffer stagingBuffer(m_Size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			indices.data());

		// Copy data from the staging buffer to this buffer
		CopyBuffer(stagingBuffer, m_Buffer, m_Size);
	}

	uint32_t IndexBuffer::GetNumberOfIndices() const
	{
		RAY_MEXPECTS(m_NumIndices != 0,
			"[IndexBuffer] Invalid number of indices. This is likely the developer's fault.");
		return m_NumIndices;
	}
}  // namespace At0::Ray
