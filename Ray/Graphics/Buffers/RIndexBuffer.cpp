#include "Rpch.h"
#include "RIndexBuffer.h"

#include "Ray/Core/RRendererLoader.h"
#include "Ray/Graphics/RGraphics.h"
#include "Ray/Graphics/Core/RLogicalDevice.h"
#include "Ray/Graphics/Commands/RCommandBuffer.h"

#include "Ray/Utils/RAssert.h"


namespace At0::Ray
{
	IndexBuffer::IndexBuffer(std::string_view tag, const std::vector<Type>& indices)
		: Buffer(sizeof(IndexBuffer::Type) * indices.size(),
			  RrBufferUsageTransferDst | RrBufferUsageIndexBuffer, RrMemoryPropertyDeviceLocal),
		  m_NumIndices((uint32_t)indices.size())
	{
		// Create staging buffer
		Buffer stagingBuffer(m_Size, RrBufferUsageTransferSrc,
			RrMemoryPropertyHostVisible | RrMemoryPropertyHostCoherent, indices.data());

		// Copy data from the staging buffer to this buffer
		CopyBuffer(stagingBuffer, m_Buffer, m_Size);
	}

	void IndexBuffer::CmdBind(const CommandBuffer& cmdBuff) const
	{
		RendererAPI::CmdBindIndexBuffer(cmdBuff, m_Buffer, 0, RrIndexTypeUInt16);
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
