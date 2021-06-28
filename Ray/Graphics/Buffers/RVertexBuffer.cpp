#include "Rpch.h"
#include "RVertexBuffer.h"

#include "Graphics/Commands/RCommandBuffer.h"
#include "Core/RDynamicVertex.h"


namespace At0::Ray
{
	VkDeviceSize VertexBuffer::s_AllocSize = 2097152;

	VertexBuffer::VertexBuffer(std::string_view tag, const DynamicVertex& vertices)
		: Buffer(vertices.SizeBytes(),
			  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	{
		// Create staging buffer
		Buffer stagingBuffer(m_Size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertices.Data());

		// Copy data from the staging buffer to this buffer
		CopyBuffer(stagingBuffer, m_Buffer, m_Size);
	}

	VertexBuffer::VertexBuffer(std::string_view tag, VkDeviceSize allocSize)
		: Buffer(allocSize ? allocSize != 0 : s_AllocSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
	{
	}

	void VertexBuffer::CmdBind(const CommandBuffer& cmdBuff) const
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(cmdBuff, 0, 1, (const VkBuffer*)&m_Buffer, offsets);
	}

	std::string VertexBuffer::GetUID(std::string_view tag, const DynamicVertex& vertices)
	{
		std::ostringstream oss;
		oss << typeid(VertexBuffer).name() << "#" << tag;
		return oss.str();
	}

	std::string VertexBuffer::GetUID(std::string_view tag, VkDeviceSize allocSize)
	{
		std::ostringstream oss;
		oss << typeid(VertexBuffer).name() << "#" << (allocSize ? allocSize != 0 : s_AllocSize)
			<< "#" << tag;
		return oss.str();
	}
}  // namespace At0::Ray
