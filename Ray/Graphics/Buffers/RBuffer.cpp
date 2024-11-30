#include "RBuffer.h"

#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RRenderContext.h"
#include "Graphics/Commands/RCommandBuffer.h"


namespace At0::Ray
{
	uint32_t Buffer::s_NonCoherentAtomSize = 0;

	Buffer::Buffer(const RenderContext& context, VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, const void* data)
		: m_Context(context), m_Size(size), m_MemoryProperties(properties)
	{
		// Check if requested memory properties are supported on this GPU
		m_MemoryProperties = ValidateMemoryProperties();

		// Set non coherent atom size
		if (s_NonCoherentAtomSize == 0)
			s_NonCoherentAtomSize =
				m_Context.physicalDevice.GetProperties().limits.nonCoherentAtomSize;

		CreateBuffer(m_Context, m_Size, usage, properties, m_Buffer, m_BufferMemory);

		if (m_MemoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			MapMemory(&m_Mapped);

		if (data)
		{
			memcpy(m_Mapped, data, m_Size);

			// If host coherent hasn't been requested, do a manual flush to make writes visible
			if (!(m_MemoryProperties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
				FlushMemory();
		}

		// Attach the memory to the buffer
		BindBufferToMemory(m_Context, m_Buffer, m_BufferMemory);
	}

	Buffer::Buffer(const RenderContext& context, VkDeviceSize size)
		: m_Context(context), m_Size(size)
	{
	}

	Buffer::Buffer(const RenderContext& context) : m_Context(context) {}

	Buffer::~Buffer()
	{
		Destroy();
	}

	void Buffer::MapMemory(void** data, VkDeviceSize size, VkDeviceSize offset) const
	{
		RAY_MEXPECTS(!m_Mapped, "[Buffer] Trying to map mapped memory");
		RAY_MEXPECTS(m_MemoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			"[Buffer] Must be host visible");
		MapMemory(m_Context, data, m_BufferMemory, size, offset);
	}

	void Buffer::UnmapMemory() const
	{
		RAY_MEXPECTS(m_Mapped, "[Buffer] Trying to unmap unmapped memory");
		UnmapMemory(m_Context, m_BufferMemory);
	}

	void Buffer::FlushMemory(VkDeviceSize size, uint32_t offset) const
	{
		FlushMemory(m_Context, m_BufferMemory, size, offset);
	}

	void Buffer::Update(void* data, VkDeviceSize size, VkDeviceSize offset)
	{
		RAY_MEXPECTS(
			offset + size < m_Size, "[Buffer] Trying to update buffer outside of buffer range");

		memcpy((char*)m_Mapped + offset, data, size);
	}

	void Buffer::CopyRange(VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size)
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

	void Buffer::MapMemory(const RenderContext& context, void** data, VkDeviceMemory memory,
		VkDeviceSize size, VkDeviceSize offset)
	{
		ThrowVulkanError(vkMapMemory(context.device, memory, offset, size, 0, data),
			"[Buffer] Failed to map memory");
	}

	void Buffer::UnmapMemory(const RenderContext& context, VkDeviceMemory memory)
	{
		vkUnmapMemory(context.device, memory);
	}

	void Buffer::FlushMemory(const RenderContext& context, VkDeviceMemory bufferMemory,
		VkDeviceSize size, uint32_t offset)
	{
		VkMappedMemoryRange mappedMemoryRange{};
		mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedMemoryRange.offset = offset;
		mappedMemoryRange.memory = bufferMemory;
		mappedMemoryRange.size = PadSizeToAlignment(size, s_NonCoherentAtomSize);
		ThrowVulkanError(vkFlushMappedMemoryRanges(context.device, 1, &mappedMemoryRange),
			"[Buffer] Failed to flush memory");
	}

	void Buffer::CreateBuffer(const RenderContext& context, VkDeviceSize size,
		VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
		VkDeviceMemory& bufferMemory)
	{
		std::array queueFamily = { context.device.GetGraphicsFamily(),
			context.device.GetPresentFamily(), context.device.GetComputeFamily() };

		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = usage;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferCreateInfo.queueFamilyIndexCount = (uint32_t)queueFamily.size();
		bufferCreateInfo.pQueueFamilyIndices = queueFamily.data();

		ThrowVulkanError(vkCreateBuffer(context.device, &bufferCreateInfo, nullptr, &buffer),
			"[Buffer] Failed to create");

		// Create the memory backing up the buffer handle
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(context.device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex =
			context.physicalDevice.FindMemoryType(memRequirements.memoryTypeBits, properties);

		ThrowVulkanError(vkAllocateMemory(context.device, &allocInfo, nullptr, &bufferMemory),
			"[Buffer] Failed to allocate buffer memory");
	}

	void Buffer::CopyBuffer(const RenderContext& context, CommandPool& transientCommandPool,
		VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		// RAY_TODO: Create separate command pool for short-lived command buffers

		CommandBuffer commandBuffer(transientCommandPool);
		commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VkBufferCopy bufferCopy{};
		bufferCopy.srcOffset = 0;
		bufferCopy.dstOffset = 0;
		bufferCopy.size = size;

		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &bufferCopy);

		commandBuffer.End();

		VkCommandBuffer cmdBuff = commandBuffer;
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuff;

		// RAY_TODO: Check which queue is faster and enable multithreading by using fences
		// instead of vkQueueWaitIdle
		vkQueueSubmit(context.device.GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(context.device.GetGraphicsQueue());
	}

	uint32_t Buffer::PadSizeToAlignment(uint32_t originalSize, uint32_t alignment)
	{
		if (alignment == 0)
			return originalSize;

		// Calculate required alignment based on minimum device offset alignment
		uint32_t alignedSize = originalSize;
		alignedSize = (alignedSize + alignment - 1) & ~(alignment - 1);
		return alignedSize;
	}

	void Buffer::Destroy()
	{
		if (m_Mapped)
			UnmapMemory();

		vkDestroyBuffer(m_Context.device, m_Buffer, nullptr);
		vkFreeMemory(m_Context.device, m_BufferMemory, nullptr);
	}

	VkMemoryPropertyFlags Buffer::ValidateMemoryProperties() const
	{
		if (!m_Context.physicalDevice.HasMemoryProperties(m_MemoryProperties))
		{
			if (m_MemoryProperties ==
				(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
			{
				Log::Warn("[Buffer] Memory properties VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | "
						  "VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT are not supported on this GPU. "
						  "Trying fallback VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | "
						  "VK_MEMORY_PROPERTY_HOST_COHERENT_BIT...");

				if (!m_Context.physicalDevice.HasMemoryProperties(
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
					ThrowRuntime(
						"[Buffer] Fallback memory properties are not supported on this GPU");

				Log::Warn(
					"[Buffer] Using fallback memory properties VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT "
					"| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT");
				return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			}
		}

		return m_MemoryProperties;
	}

	void Buffer::BindBufferToMemory(
		const RenderContext& context, VkBuffer buffer, VkDeviceMemory memory)
	{
		// Attach the memory to the buffer
		ThrowVulkanError(vkBindBufferMemory(context.device, buffer, memory, 0),
			"[Buffer] Failed to map buffer to buffer memory");
	}
}  // namespace At0::Ray
