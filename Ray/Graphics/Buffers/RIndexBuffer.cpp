#include "Rpch.h"
#include "RIndexBuffer.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"


namespace At0::Ray
{
	IndexBuffer::IndexBuffer(std::string_view tag, const std::vector<Type>& indices)
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

	void IndexBuffer::CmdBind(const CommandBuffer& cmdBuff) const
	{
		vkCmdBindIndexBuffer(cmdBuff, m_Buffer, 0, VK_INDEX_TYPE_UINT16);
	}

	uint32_t IndexBuffer::GetNumberOfIndices() const
	{
		RAY_MEXPECTS(m_NumIndices != 0,
			"[IndexBuffer] Invalid number of indices. This is likely the developer's fault");
		return m_NumIndices;
	}

	std::string IndexBuffer::GetUID(
		std::string_view tag, const std::vector<IndexBuffer::Type>& indices)
	{
		std::ostringstream oss;
		oss << typeid(IndexBuffer).name() << "#" << tag;
		return oss.str();
	}

}  // namespace At0::Ray
