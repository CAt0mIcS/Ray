#include "Rpch.h"
#include "RBuffer.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"

#include "Utils/RException.h"
#include "Utils/RAssert.h"


namespace At0::Ray
{
	Buffer::Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		const void* data)
		: m_Size(size), m_IsHostCoherent((properties & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0)
	{
		CreateBuffer(m_Size, usage, properties, m_Buffer, m_BufferMemory);

		if (data)
		{
			void* mapped;
			MapMemory(&mapped);
			memcpy(mapped, data, m_Size);

			// If host coherent hasn't been requested, do a manual flush t omake writes visible
			if (!m_IsHostCoherent)
				FlushMemory();

			UnmapMemory();
		}

		// Attach the memory to the buffer
		RAY_VK_THROW_FAILED(
			vkBindBufferMemory(Graphics::Get().GetDevice(), m_Buffer, m_BufferMemory, 0),
			"[Buffer] Failed to map buffer to buffer memory.");
	}

	Buffer::Buffer(VkDeviceSize size) : m_Size(size) {}

	Buffer::~Buffer() { Destroy(); }

	void Buffer::MapMemory(void** data) const { MapMemory(data, m_BufferMemory, m_Size); }

	void Buffer::UnmapMemory() const { UnmapMemory(m_BufferMemory); }

	void Buffer::FlushMemory() const { FlushMemory(m_BufferMemory, m_Size); }

	void Buffer::MapMemory(void** data, VkDeviceMemory memory, VkDeviceSize size)
	{
		vkMapMemory(Graphics::Get().GetDevice(), memory, 0, size, 0, data);
	}

	void Buffer::UnmapMemory(VkDeviceMemory memory)
	{
		vkUnmapMemory(Graphics::Get().GetDevice(), memory);
	}

	void Buffer::FlushMemory(VkDeviceMemory bufferMemory, VkDeviceSize size)
	{
		VkMappedMemoryRange mappedMemoryRange{};
		mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedMemoryRange.offset = 0;
		mappedMemoryRange.memory = bufferMemory;
		mappedMemoryRange.size = size;
		RAY_VK_THROW_FAILED(
			vkFlushMappedMemoryRanges(Graphics::Get().GetDevice(), 1, &mappedMemoryRange),
			"[Buffer] Failed to flush memory.");
	}

	void Buffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		std::array queueFamily = { Graphics::Get().GetDevice().GetGraphicsFamily(),
			Graphics::Get().GetDevice().GetPresentFamily(),
			Graphics::Get().GetDevice().GetComputeFamily() };

		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = usage;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferCreateInfo.queueFamilyIndexCount = (uint32_t)queueFamily.size();
		bufferCreateInfo.pQueueFamilyIndices = queueFamily.data();

		RAY_VK_THROW_FAILED(
			vkCreateBuffer(Graphics::Get().GetDevice(), &bufferCreateInfo, nullptr, &buffer),
			"[Buffer] Failed to create.");

		// Create the memory backing up the buffer handle
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(Graphics::Get().GetDevice(), buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Graphics::Get().GetPhysicalDevice().FindMemoryType(
			memRequirements.memoryTypeBits, properties);

		RAY_VK_THROW_FAILED(
			vkAllocateMemory(Graphics::Get().GetDevice(), &allocInfo, nullptr, &bufferMemory),
			"[Buffer] Failed to allocate buffer memory.");
	}

	void Buffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		// RAY_TODO: Create separate command pool for short-lived command buffers

		CommandBuffer commandBuffer(Graphics::Get().GetCommandPool());
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
		vkQueueSubmit(
			Graphics::Get().GetDevice().GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(Graphics::Get().GetDevice().GetGraphicsQueue());
	}

	uint32_t Buffer::PadSizeToAlignment(uint32_t originalSize, uint32_t alignment)
	{
		// Calculate required alignment based on minimum device offset alignment
		uint32_t alignedSize = originalSize;
		if (alignment > 0)
		{
			alignedSize = (alignedSize + alignment - 1) & ~(alignment - 1);
		}
		return alignedSize;
	}

	void Buffer::Destroy()
	{
		vkDestroyBuffer(Graphics::Get().GetDevice(), m_Buffer, nullptr);
		vkFreeMemory(Graphics::Get().GetDevice(), m_BufferMemory, nullptr);
	}

	void Buffer::BindBufferToMemory(VkBuffer buffer, VkDeviceMemory memory)
	{  // Attach the memory to the buffer
		RAY_VK_THROW_FAILED(vkBindBufferMemory(Graphics::Get().GetDevice(), buffer, memory, 0),
			"[Buffer] Failed to map buffer to buffer memory.");
	}
}  // namespace At0::Ray
