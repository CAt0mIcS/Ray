#include "Rpch.h"
#include "RVertexBuffer.h"


namespace At0::Ray
{
	VertexBuffer::VertexBuffer(const std::vector<Float3>& vertices)
		: Buffer(sizeof(Float3) * vertices.size(),
			  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	{
		// Create staging buffer
		Buffer stagingBuffer(m_Size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertices.data());

		// Copy data from the staging buffer to this buffer
		CopyBuffer(stagingBuffer, m_Buffer, m_Size);
	}
}  // namespace At0::Ray
