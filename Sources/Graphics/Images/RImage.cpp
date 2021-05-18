#include "Rpch.h"
#include "RImage.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Buffers/RBuffer.h"

#include "Utils/RLogger.h"
#include "Utils/RException.h"


namespace At0::Ray
{
	Image::Image(UInt2 extent, VkImageType imageType, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags memProps, uint32_t mipLevels,
		VkImageAspectFlags imageAspect, uint32_t arrayLayers, VkImageCreateFlags createFlags)
		: m_Extent(extent), m_ImageType(imageType), m_Format(format), m_Tiling(tiling),
		  m_Usage(usage), m_MemoryProperties(memProps), m_MipLevels(mipLevels),
		  m_ImageAspect(imageAspect), m_ArrayLayers(arrayLayers), m_CreateFlags(createFlags)
	{
		Setup();
	}

	Image::~Image()
	{
		vkDestroyImage(Graphics::Get().GetDevice(), m_Image, nullptr);
		vkFreeMemory(Graphics::Get().GetDevice(), m_ImageMemory, nullptr);
	}

	void Image::TransitionLayout(VkImageLayout newLayout)
	{
		TransitionLayout(m_Image, m_ImageLayout, newLayout, m_MipLevels, m_ArrayLayers);
		m_ImageLayout = newLayout;
	}

	bool Image::GenerateMipmaps()
	{
		RAY_MEXPECTS(m_Usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
			"[Image] Cannot generate mipmaps if the image was not created with usage flag "
			"VK_IMAGE_USAGE_TRANSFER_SRC_BIT");

		if (GenerateMipmaps(m_Image, m_Format, m_Extent.x, m_Extent.y, m_MipLevels))
		{
			m_ImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			return true;
		}
		return false;
	}

	void Image::TransitionLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout,
		uint32_t mipLevels, uint32_t layerCount)
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
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = layerCount;

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

		// RAY_TODO: Get best queue
		commandBuffer.Submit(Graphics::Get().GetDevice().GetGraphicsQueue());
		vkQueueWaitIdle(Graphics::Get().GetDevice().GetGraphicsQueue());
	}

	bool Image::GenerateMipmaps(
		VkImage image, VkFormat imageFormat, int32_t width, int32_t height, uint32_t mipLevels)
	{
		VkFormatProperties formatProps;
		vkGetPhysicalDeviceFormatProperties(
			Graphics::Get().GetPhysicalDevice(), imageFormat, &formatProps);

		if (!(formatProps.optimalTilingFeatures &
				VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
		{
			Log::Error("[Image] Usable to generate mipmaps because "
					   "VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT is missing");
			return false;
		}

		CommandBuffer cmdBuff(Graphics::Get().GetCommandPool());
		cmdBuff.Begin();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipWidth = width;
		int32_t mipHeight = height;

		for (uint32_t i = 1; i < mipLevels; ++i)
		{
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(cmdBuff, VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			VkImageBlit blit{};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1,
				mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(cmdBuff, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(cmdBuff, VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			if (mipWidth > 1)
				mipWidth /= 2;
			if (mipHeight > 1)
				mipHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(cmdBuff, VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		cmdBuff.End();

		// RAY_TODO: Transfer queue with graphics capabilities is faster
		cmdBuff.Submit(Graphics::Get().GetDevice().GetGraphicsQueue());
		vkQueueWaitIdle(Graphics::Get().GetDevice().GetGraphicsQueue());
		return true;
	}


	void Image::CopyFromBuffer(const Buffer& buffer, std::vector<VkBufferImageCopy> copyRegions)
	{
		if (m_ImageLayout != VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		CommandBuffer commandBuffer(Graphics::Get().GetCommandPool());
		commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;	 // RAY_TODO: MipLevel

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { m_Extent.x, m_Extent.y, 1 };

		if (copyRegions.empty())
			copyRegions.emplace_back(region);

		vkCmdCopyBufferToImage(commandBuffer, buffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			(uint32_t)copyRegions.size(), copyRegions.data());
		commandBuffer.End();

		// RAY_TODO: Get best queue
		commandBuffer.Submit(Graphics::Get().GetDevice().GetGraphicsQueue());
		vkQueueWaitIdle(Graphics::Get().GetDevice().GetGraphicsQueue());
	}

	Image& Image::operator=(Image&& other) noexcept
	{
		m_Extent = std::move(other.m_Extent);
		m_Format = std::move(other.m_Format);
		m_ImageLayout = std::move(other.m_ImageLayout);
		m_MipLevels = std::move(other.m_MipLevels);
		Setup();
		return *this;
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

	void Image::Setup()
	{
		std::array queueFamily = { Graphics::Get().GetDevice().GetGraphicsFamily(),
			Graphics::Get().GetDevice().GetPresentFamily(),
			Graphics::Get().GetDevice().GetComputeFamily() };

		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.flags = m_CreateFlags;
		imageCreateInfo.imageType = m_ImageType;
		imageCreateInfo.format = m_Format;
		imageCreateInfo.extent = VkExtent3D{ m_Extent.x, m_Extent.y, 1 };
		imageCreateInfo.mipLevels = m_MipLevels;
		imageCreateInfo.arrayLayers = m_ArrayLayers;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = m_Tiling;
		imageCreateInfo.usage = m_Usage;
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
			memRequirements.memoryTypeBits, m_MemoryProperties);

		RAY_VK_THROW_FAILED(
			vkAllocateMemory(Graphics::Get().GetDevice(), &allocInfo, nullptr, &m_ImageMemory),
			"[Image] Failed to allocate image memory");

		vkBindImageMemory(Graphics::Get().GetDevice(), m_Image, m_ImageMemory, 0);

		m_ImageView = MakeScope<ImageView>(*this);
	}
}  // namespace At0::Ray
