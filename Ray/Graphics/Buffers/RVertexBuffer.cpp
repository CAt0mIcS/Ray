#include "Rpch.h"
#include "RVertexBuffer.h"

#include "Ray/Core/RRendererLoader.h"
#include "Ray/Core/RDynamicVertex.h"
#include "Ray/Graphics/Commands/RCommandBuffer.h"


namespace At0::Ray
{
	RrDeviceSize VertexBuffer::s_AllocSize = 2097152;

	VertexBuffer::VertexBuffer(std::string_view tag, const DynamicVertex& vertices)
		: Buffer(vertices.SizeBytes(), RrBufferUsageTransferDst | RrBufferUsageVertexBuffer,
			  RrMemoryPropertyDeviceLocal)
	{
		// Create staging buffer
		Buffer stagingBuffer(m_Size, RrBufferUsageTransferSrc,
			RrMemoryPropertyHostVisible | RrMemoryPropertyHostCoherent, vertices.Data());

		// Copy data from the staging buffer to this buffer
		CopyBuffer(stagingBuffer, m_Buffer, m_Size);
	}

	VertexBuffer::VertexBuffer(std::string_view tag, RrDeviceSize allocSize)
		: Buffer(allocSize ? allocSize != 0 : s_AllocSize, RrBufferUsageVertexBuffer,
			  RrMemoryPropertyDeviceLocal | RrMemoryPropertyHostVisible)
	{
	}

	void VertexBuffer::CmdBind(const CommandBuffer& cmdBuff) const
	{
		RrDeviceSize offsets[] = { 0 };
		RendererAPI::CmdBindVertexBuffers(cmdBuff, 0, 1, &m_Buffer, offsets);
	}

	std::string VertexBuffer::GetUID(std::string_view tag, const DynamicVertex& vertices)
	{
		std::ostringstream oss;
		oss << typeid(VertexBuffer).name() << "#" << tag;
		return oss.str();
	}

	std::string VertexBuffer::GetUID(std::string_view tag, RrDeviceSize allocSize)
	{
		std::ostringstream oss;
		oss << typeid(VertexBuffer).name() << "#" << (allocSize ? allocSize != 0 : s_AllocSize)
			<< "#" << tag;
		return oss.str();
	}
}  // namespace At0::Ray
