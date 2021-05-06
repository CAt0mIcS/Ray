#include "Rpch.h"
#include "RImage.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Buffers/RBuffer.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	Image::Image(UInt2 extent, VkImageType imageType, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags memProps, VkImageAspectFlags imageAspect)
		: m_Format(format), m_Extent(extent)
	{
		std::array queueFamily = { Graphics::Get().GetDevice().GetGraphicsFamily(),
			Graphics::Get().GetDevice().GetPresentFamily(),
			Graphics::Get().GetDevice().GetComputeFamily() };

		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.flags = 0;
		imageCreateInfo.imageType = imageType;
		imageCreateInfo.format = format;
		imageCreateInfo.extent = VkExtent3D{ extent.x, extent.y, 1 };
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = tiling;
		imageCreateInfo.usage = usage;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.queueFamilyIndexCount = (uint32_t)queueFamily.size();
		imageCreateInfo.pQueueFamilyIndices = queueFamily.data();
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		RAY_VK_THROW_FAILED(
			vkCreateImage(Graphics::Get().GetDevice(), &imageCreateInfo, nullptr, &m_Image),
			"[Image] Failed to create");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(Graphics::Get().GetDevice(), m_Image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Graphics::Get().GetPhysicalDevice().FindMemoryType(
			memRequirements.memoryTypeBits, memProps);

		RAY_VK_THROW_FAILED(
			vkAllocateMemory(Graphics::Get().GetDevice(), &allocInfo, nullptr, &m_ImageMemory),
			"[Image] Failed to allocate image memory");

		vkBindImageMemory(Graphics::Get().GetDevice(), m_Image, m_ImageMemory, 0);

		m_ImageView = MakeScope<ImageView>(m_Image, format, imageAspect);
	}

	Image::~Image()
	{
		vkDestroyImage(Graphics::Get().GetDevice(), m_Image, nullptr);
		vkFreeMemory(Graphics::Get().GetDevice(), m_ImageMemory, nullptr);
	}

	void Image::TransitionLayout(VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		TransitionLayout(m_Image, oldLayout, newLayout);
	}

	void Image::TransitionLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		CommandBuffer commandBuffer(Graphics::Get().GetCommandPool());
		commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
			newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
				 newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else
			RAY_THROW_RUNTIME("[Image] Invalid or unsupported combination of old and new layout");

		vkCmdPipelineBarrier(
			commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		commandBuffer.End();

		VkCommandBuffer cmdBuff = commandBuffer;
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuff;

		// RAY_TODO: Get best queue
		vkQueueSubmit(
			Graphics::Get().GetDevice().GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(Graphics::Get().GetDevice().GetGraphicsQueue());
	}

	void Image::CopyFromBuffer(const Buffer& buffer)
	{
		CommandBuffer commandBuffer(Graphics::Get().GetCommandPool());
		commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { m_Extent.x, m_Extent.y, 1 };

		vkCmdCopyBufferToImage(
			commandBuffer, buffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		commandBuffer.End();

		VkCommandBuffer cmdBuff = commandBuffer;
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuff;

		// RAY_TODO: Get best queue
		vkQueueSubmit(
			Graphics::Get().GetDevice().GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(Graphics::Get().GetDevice().GetGraphicsQueue());
	}

	std::vector<VkFormat> Image::FindSupportedFormats(
		std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags featureFlags)
	{
		for (int32_t i = candidates.size() - 1; i >= 0; --i)
		{
			VkFormatProperties formatProps;
			vkGetPhysicalDeviceFormatProperties(
				Graphics::Get().GetPhysicalDevice(), candidates[i], &formatProps);

			if (tiling == VK_IMAGE_TILING_LINEAR &&
				(formatProps.linearTilingFeatures & featureFlags) != featureFlags)
			{
				candidates.erase(candidates.begin() + i);
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
					 (formatProps.optimalTilingFeatures & featureFlags) != featureFlags)
			{
				candidates.erase(candidates.begin() + i);
			}
		}

		return candidates;
	}
}  // namespace At0::Ray
