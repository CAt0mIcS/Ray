#include "Rpch.h"
#include "RBuffer.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"

#include "RayBase/RException.h"
#include "RayBase/RAssert.h"
#include "RayBase/RLogger.h"

#include "Core/RRendererLoader.h"


namespace At0::Ray
{
	uint32_t Buffer::s_NonCoherentAtomSize = 0;

	Buffer::Buffer(RrDeviceSize size, RrBufferUsageFlags usage, RrMemoryPropertyFlags properties,
		const void* data)
		: m_Size(size), m_MemoryProperties(properties)
	{
		// Check if requested memory properties are supported on this GPU
		m_MemoryProperties = ValidateMemoryProperties();

		// Set non coherent atom size
		if (s_NonCoherentAtomSize == 0)
			s_NonCoherentAtomSize =
				Graphics::Get().GetPhysicalDevice().GetProperties().limits.nonCoherentAtomSize;

		CreateBuffer(m_Size, usage, properties, m_Buffer, m_BufferMemory);

		if (m_MemoryProperties & RrMemoryPropertyHostVisible)
			MapMemory(&m_Mapped);

		if (data)
		{
			memcpy(m_Mapped, data, m_Size);

			// If host coherent hasn't been requested, do a manual flush to make writes visible
			if (!(m_MemoryProperties & RrMemoryPropertyHostCoherent))
				FlushMemory();
		}

		// Attach the memory to the buffer
		BindBufferToMemory(m_Buffer, m_BufferMemory);
	}

	Buffer::Buffer(RrDeviceSize size) : m_Size(size) {}

	Buffer::~Buffer() { Destroy(); }

	void Buffer::MapMemory(void** data, RrDeviceSize size, RrDeviceSize offset) const
	{
		RAY_MEXPECTS(!m_Mapped, "[Buffer] Trying to map mapped memory");
		RAY_MEXPECTS(
			m_MemoryProperties & RrMemoryPropertyHostVisible, "[Buffer] Must be host visible");
		MapMemory(data, m_BufferMemory, size, offset);
	}

	void Buffer::UnmapMemory() const
	{
		RAY_MEXPECTS(m_Mapped, "[Buffer] Trying to unmap unmapped memory");
		UnmapMemory(m_BufferMemory);
	}

	void Buffer::FlushMemory(RrDeviceSize size, RrDeviceSize offset) const
	{
		FlushMemory(m_BufferMemory, size, offset);
	}

	void Buffer::Update(void* data, RrDeviceSize size, RrDeviceSize offset)
	{
		RAY_MEXPECTS(
			offset + size < m_Size, "[Buffer] Trying to update buffer outside of buffer range");

		memcpy((char*)m_Mapped + offset, data, size);
	}

	void Buffer::CopyRange(RrDeviceSize srcOffset, RrDeviceSize dstOffset, RrDeviceSize size)
	{
		RAY_ASSERT(
			false, "[Buffer] Buffer::CopyRange is untested! Remove this if it works as expected");
		RAY_MEXPECTS(srcOffset + size < m_Size && dstOffset + size < m_Size,
			"[Buffer] Trying to copy buffer data outside of buffer range");

		void* srcMapped;
		MapMemory(&srcMapped, size, srcOffset);

		void* dstMapped;
		MapMemory(&dstMapped, size, dstOffset);

		memcpy(dstMapped, srcMapped, size);

		UnmapMemory();
	}

	void Buffer::MapMemory(
		void** data, RrDeviceMemory memory, RrDeviceSize size, RrDeviceSize offset)
	{
		ThrowRenderError(
			RendererAPI::MapMemory(Graphics::Get().GetDevice(), memory, offset, size, data),
			"[Buffer] Failed to map memory");
	}

	void Buffer::UnmapMemory(RrDeviceMemory memory)
	{
		RendererAPI::UnmapMemory(Graphics::Get().GetDevice(), memory);
	}

	void Buffer::FlushMemory(RrDeviceMemory bufferMemory, RrDeviceSize size, RrDeviceSize offset)
	{
		RrMappedMemoryRange mappedMemoryRange{};
		mappedMemoryRange.offset = offset;
		mappedMemoryRange.size = PadSizeToAlignment(size, s_NonCoherentAtomSize);
		mappedMemoryRange.memory = bufferMemory;

		ThrowRenderError(RendererAPI::FlushMappedMemoryRanges(
							 Graphics::Get().GetDevice(), 1, &mappedMemoryRange),
			"[Buffer] Failed to flush memory");
	}

	void Buffer::CreateBuffer(RrDeviceSize size, RrBufferUsageFlags usage,
		RrMemoryPropertyFlags properties, RrBuffer& buffer, RrDeviceMemory& bufferMemory)
	{
		std::array queueFamily = { Graphics::Get().GetDevice().GetGraphicsFamily(),
			Graphics::Get().GetDevice().GetPresentFamily(),
			Graphics::Get().GetDevice().GetComputeFamily() };

		RrBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = usage;
		bufferCreateInfo.sharingMode = RrSharingModeExclusive;
		bufferCreateInfo.queueFamilyIndexCount = (uint32_t)queueFamily.size();
		bufferCreateInfo.pQueueFamilyIndices = queueFamily.data();

		ThrowRenderError(
			RendererAPI::CreateBuffer(Graphics::Get().GetDevice(), &bufferCreateInfo, &buffer),
			"[Buffer] Failed to create");

		// Create the memory backing up the buffer handle
		RrMemoryRequirements memRequirements;
		RendererAPI::BufferGetMemoryRequirements(
			Graphics::Get().GetDevice(), buffer, &memRequirements);

		RrMemoryAllocateInfo allocInfo{};
		allocInfo.allocationSize = memRequirements.size;
		RendererAPI::DeviceMemoryGetMemoryTypeIndex(memRequirements.memoryTypeBits, properties,
			Graphics::Get().GetPhysicalDevice().GetMemoryProperties().memoryTypeCount,
			Graphics::Get().GetPhysicalDevice().GetMemoryProperties().memoryTypes,
			&allocInfo.memoryTypeIndex);

		ThrowRenderError(
			RendererAPI::AllocateMemory(Graphics::Get().GetDevice(), &allocInfo, &bufferMemory),
			"[Buffer] Failed to allocate buffer memory");
	}

	void Buffer::CopyBuffer(RrBuffer srcBuffer, RrBuffer dstBuffer, RrDeviceSize size)
	{
		// RAY_TODO: Create separate command pool for short-lived command buffers

		CommandBuffer commandBuffer(Graphics::Get().GetCommandPool());
		commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VkBufferCopy bufferCopy{};
		bufferCopy.srcOffset = 0;
		bufferCopy.dstOffset = 0;
		bufferCopy.size = size;

		vkCmdCopyBuffer(commandBuffer, (VkBuffer)srcBuffer, (VkBuffer)dstBuffer, 1, &bufferCopy);

		commandBuffer.End();

		RrCommandBuffer cmdBuff = commandBuffer;
		RrSubmitInfo submitInfo{};
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuff;

		// RAY_TODO: Check which queue is faster and enable multithreading by using fences
		// instead of vkQueueWaitIdle
		RendererAPI::QueueSubmit(
			Graphics::Get().GetDevice().GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		RendererAPI::QueueWaitIdle(Graphics::Get().GetDevice().GetGraphicsQueue());
	}

	uint32_t Buffer::PadSizeToAlignment(RrDeviceSize originalSize, RrDeviceSize alignment)
	{
		if (alignment == 0)
			return originalSize;

		// Calculate required alignment based on minimum device offset alignment
		RrDeviceSize alignedSize = originalSize;
		alignedSize = (alignedSize + alignment - 1) & ~(alignment - 1);
		return alignedSize;
	}

	void Buffer::Destroy()
	{
		if (m_Mapped)
			UnmapMemory();

		RendererAPI::DestroyBuffer(Graphics::Get().GetDevice(), m_Buffer);
		RendererAPI::FreeMemory(Graphics::Get().GetDevice(), m_BufferMemory);
	}

	RrMemoryPropertyFlags Buffer::ValidateMemoryProperties() const
	{
		if (!Graphics::Get().GetPhysicalDevice().HasMemoryProperties(m_MemoryProperties))
		{
			if (m_MemoryProperties == (RrMemoryPropertyDeviceLocal | RrMemoryPropertyHostVisible))
			{
				Log::Warn("[Buffer] Memory properties RrMemoryPropertyDeviceLocal | "
						  "RrMemoryPropertyHostVisible are not supported on this GPU. "
						  "Trying fallback RrMemoryPropertyHostVisible | "
						  "RrMemoryPropertyHostCoherent...");

				if (!Graphics::Get().GetPhysicalDevice().HasMemoryProperties(
						RrMemoryPropertyHostVisible | RrMemoryPropertyHostCoherent))
					ThrowRuntime(
						"[Buffer] Fallback memory properties are not supported on this GPU");

				Log::Warn("[Buffer] Using fallback memory properties RrMemoryPropertyHostVisible "
						  "| RrMemoryPropertyHostCoherent");
				return RrMemoryPropertyHostVisible | RrMemoryPropertyHostCoherent;
			}
		}

		return m_MemoryProperties;
	}

	void Buffer::BindBufferToMemory(RrBuffer buffer, RrDeviceMemory memory)
	{
		// Attach the memory to the buffer
		ThrowRenderError(
			RendererAPI::BindBufferMemory(Graphics::Get().GetDevice(), buffer, memory, 0),
			"[Buffer] Failed to map buffer to buffer memory");
	}
}  // namespace At0::Ray
