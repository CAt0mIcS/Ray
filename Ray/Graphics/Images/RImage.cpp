#include "RImage.h"

#include "Core/RResourceManager.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Buffers/RBuffer.h"
#include "RImageView.h"
#include "Graphics/Core/RRenderContext.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>


namespace At0::Ray
{
	Ref<Image> Image::Acquire(UInt2 extent, VkImageType imageType, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memProps,
		uint32_t mipLevels, VkImageAspectFlags imageAspect, uint32_t arrayLayers,
		VkImageCreateFlags createFlags)
	{
		// RAY_TODO: Add data to ressource tag

		return ResourceManager::Get().EmplaceOrGet<Image>(
			String::Serialize("Image{0}{1}{2}{3}{4}{5}{6}{7}{8}{9}{10}", extent.x, extent.y,
				(uint32_t)imageType, (uint32_t)format, (uint32_t)tiling, (uint32_t)usage,
				(uint32_t)memProps, mipLevels, (uint32_t)imageAspect, arrayLayers,
				(uint32_t)createFlags),
			std::move(extent), imageType, format, tiling, usage, memProps, mipLevels, imageAspect,
			arrayLayers, createFlags);
	}

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
		vkDestroyImage(Graphics::Get().GetRenderContext().device, m_Image, nullptr);
		vkFreeMemory(Graphics::Get().GetRenderContext().device, m_ImageMemory, nullptr);
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

	void Image::WritePPM(std::string_view filepath)
	{
		bool supportsBlit = true;
		VkFormatProperties formatProps;

		// Check if the device supports blitting from optimal/linear images
		vkGetPhysicalDeviceFormatProperties(
			Graphics::Get().GetRenderContext().physicalDevice, m_Format, &formatProps);
		if (!(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT) ||
			!(formatProps.linearTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT))
		{
			Log::Warn("[Image] Device does not support blitting from optimal/linear tiled images, "
					  "using copy instead of blit");
			supportsBlit = false;
		}

		// Create the linear tiled destination image to copy to and to read the memory from
		Image dstImage(m_Extent, VK_IMAGE_TYPE_2D, m_Format, VK_IMAGE_TILING_LINEAR,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		CommandBuffer cmdBuff(Graphics::Get().GetCommandPool());
		cmdBuff.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		dstImage.TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		VkImageLayout oldLayout = m_ImageLayout;
		TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

		if (supportsBlit)
		{
			// Define region to blit
			VkOffset3D blitSize{};
			blitSize.x = m_Extent.x;
			blitSize.y = m_Extent.y;
			blitSize.z = 1;
			VkImageBlit imageBlitRegion{};
			imageBlitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlitRegion.srcSubresource.layerCount = 1;
			imageBlitRegion.srcOffsets[1] = blitSize;
			imageBlitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlitRegion.dstSubresource.layerCount = 1;
			imageBlitRegion.dstOffsets[1] = blitSize;

			// Issue the blit command
			vkCmdBlitImage(cmdBuff, m_Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlitRegion, VK_FILTER_NEAREST);
		}
		else
		{
			// Otherwise use image copy (requires us to manually flip components)
			VkImageCopy imageCopyRegion{};
			imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.srcSubresource.layerCount = 1;
			imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.dstSubresource.layerCount = 1;
			imageCopyRegion.extent.width = m_Extent.x;
			imageCopyRegion.extent.height = m_Extent.y;
			imageCopyRegion.extent.depth = 1;

			// Issue the copy command
			vkCmdCopyImage(cmdBuff, m_Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);
		}

		cmdBuff.End();
		// RAY_TODO: Get best queue
		cmdBuff.Submit(Graphics::Get().GetRenderContext().device.GetGraphicsQueue());
		vkQueueWaitIdle(Graphics::Get().GetRenderContext().device.GetGraphicsQueue());

		dstImage.TransitionLayout(VK_IMAGE_LAYOUT_GENERAL);
		TransitionLayout(oldLayout);

		// Get layout of the image (including row pitch)
		VkImageSubresource subResource{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 0 };
		VkSubresourceLayout subResourceLayout;
		vkGetImageSubresourceLayout(
			Graphics::Get().GetRenderContext().device, dstImage, &subResource, &subResourceLayout);
		const char* data = nullptr;
		vkMapMemory(Graphics::Get().GetRenderContext().device, dstImage.GetImageMemory(), 0,
			VK_WHOLE_SIZE, 0, (void**)&data);
		data += subResourceLayout.offset;

		std::ofstream file(filepath.data(), std::ios::out | std::ios::binary);

		// ppm header
		file << "P6\n" << m_Extent.x << "\n" << m_Extent.y << "\n" << 255 << "\n";

		// If source is BGR (destination is always RGB) and we can't use blit (which does automatic
		// conversion), we'll have to manually swizzle color components
		bool colorSwizzle = false;
		// Check if source is BGR
		// Note: Not complete, only contains most common and basic BGR surface formats for
		// demonstration purposes
		if (!supportsBlit)
		{
			std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM,
				VK_FORMAT_B8G8R8A8_SNORM };
			colorSwizzle =
				(std::find(formatsBGR.begin(), formatsBGR.end(), m_Format) != formatsBGR.end());
		}

		// ppm binary pixel data
		for (uint32_t y = 0; y < m_Extent.y; y++)
		{
			unsigned int* row = (unsigned int*)data;
			for (uint32_t x = 0; x < m_Extent.x; x++)
			{
				if (colorSwizzle)
				{
					file.write((char*)row + 2, 1);
					file.write((char*)row + 1, 1);
					file.write((char*)row, 1);
				}
				else
					file.write((char*)row, 3);
				row++;
			}
			data += subResourceLayout.rowPitch;
		}
		file.close();
	}

	void Image::TransitionLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout,
		uint32_t mipLevels, uint32_t layerCount)
	{
		if (oldLayout == newLayout)
			return;

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

		switch (oldLayout)
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			barrier.srcAccessMask = 0;
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			break;
		default:
			ThrowRuntime("[Image] Invalid or unsupported old layout ({0})", (uint32_t)oldLayout);
		}

		switch (newLayout)
		{
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			break;
		case VK_IMAGE_LAYOUT_GENERAL:
			barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			break;

		default:
			ThrowRuntime("[Image] Invalid or unsupported new layout ({0})", (uint32_t)newLayout);
		}


		vkCmdPipelineBarrier(
			commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		commandBuffer.End();

		// RAY_TODO: Get best queue
		commandBuffer.Submit(Graphics::Get().GetRenderContext().device.GetGraphicsQueue());
		vkQueueWaitIdle(Graphics::Get().GetRenderContext().device.GetGraphicsQueue());
	}

	bool Image::GenerateMipmaps(
		VkImage image, VkFormat imageFormat, int32_t width, int32_t height, uint32_t mipLevels)
	{
		VkFormatProperties formatProps;
		vkGetPhysicalDeviceFormatProperties(
			Graphics::Get().GetRenderContext().physicalDevice, imageFormat, &formatProps);

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
		cmdBuff.Submit(Graphics::Get().GetRenderContext().device.GetGraphicsQueue());
		vkQueueWaitIdle(Graphics::Get().GetRenderContext().device.GetGraphicsQueue());
		return true;
	}  // namespace At0::Ray


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
		commandBuffer.Submit(Graphics::Get().GetRenderContext().device.GetGraphicsQueue());
		vkQueueWaitIdle(Graphics::Get().GetRenderContext().device.GetGraphicsQueue());
	}

	void Image::CopyFromData(const std::vector<uint8_t>& data)
	{
		Buffer stagingBuffer(data.size() * sizeof(uint8_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			data.data());
		CopyFromBuffer(stagingBuffer);
	}

	void Image::CopyFromData(const void* data, uint32_t size)
	{
		Buffer stagingBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, data);
		CopyFromBuffer(stagingBuffer);
	}

	Buffer&& Image::CopyToBuffer(std::vector<VkBufferImageCopy> copyRegions)
	{
		RAY_MEXPECTS(m_Usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
			"[Image] Image must be created with VK_IMAGE_USAGE_TRANSFER_SRC_BIT if it should be "
			"copyable to a buffer");

		// RAY_TODO: Check if VK_FORMAT_FEATURE_TRANSFER_SRC_BIT is set
		VkImageLayout oldLayout = m_ImageLayout;
		TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

		CommandBuffer cmdBuff(Graphics::Get().GetCommandPool());
		cmdBuff.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

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

		Buffer dstBuffer(m_Extent.x * m_Extent.y * 4, VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		vkCmdCopyImageToBuffer(cmdBuff, m_Image, m_ImageLayout, dstBuffer,
			(uint32_t)copyRegions.size(), copyRegions.data());

		cmdBuff.End();

		// RAY_TODO: Get best queue
		cmdBuff.Submit(Graphics::Get().GetRenderContext().device.GetGraphicsQueue());
		vkQueueWaitIdle(Graphics::Get().GetRenderContext().device.GetGraphicsQueue());

		TransitionLayout(oldLayout);
		return std::move(dstBuffer);
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

	Image::Image(Image&& other) noexcept
	{
		*this = std::move(other);
	}

	std::vector<VkFormat> Image::FindSupportedFormats(
		std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags featureFlags)
	{
		for (int32_t i = candidates.size() - 1; i >= 0; --i)
		{
			VkFormatProperties formatProps;
			vkGetPhysicalDeviceFormatProperties(
				Graphics::Get().GetRenderContext().physicalDevice, candidates[i], &formatProps);

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
		std::array queueFamily = { Graphics::Get().GetRenderContext().device.GetGraphicsFamily(),
			Graphics::Get().GetRenderContext().device.GetPresentFamily(),
			Graphics::Get().GetRenderContext().device.GetComputeFamily() };

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

		ThrowVulkanError(vkCreateImage(Graphics::Get().GetRenderContext().device, &imageCreateInfo,
							 nullptr, &m_Image),
			"[Image] Failed to create");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(
			Graphics::Get().GetRenderContext().device, m_Image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex =
			Graphics::Get().GetRenderContext().physicalDevice.FindMemoryType(
				memRequirements.memoryTypeBits, m_MemoryProperties);

		ThrowVulkanError(vkAllocateMemory(Graphics::Get().GetRenderContext().device, &allocInfo,
							 nullptr, &m_ImageMemory),
			"[Image] Failed to allocate image memory");

		vkBindImageMemory(Graphics::Get().GetRenderContext().device, m_Image, m_ImageMemory, 0);

		// Create image view only for supported image usages
		if ((m_Usage & VK_IMAGE_USAGE_SAMPLED_BIT) || (m_Usage & VK_IMAGE_USAGE_STORAGE_BIT) ||
			(m_Usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) ||
			(m_Usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) ||
			(m_Usage & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) ||
			(m_Usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) ||
			(m_Usage & VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV) ||
			(m_Usage & VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT))
			m_ImageView = MakeScope<ImageView>(*this);
	}

	Image::Image() {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////// BUILDER //////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Ref<Image> Image::Builder::Build()
	{
		ThrowIfInvalidArguments();
		return MakeRef<Image>(m_Extent, m_ImageType, m_Format, m_Tiling, m_Usage,
			m_MemoryProperties, m_MipLevels, m_ImageAspect, m_ArrayLayers, m_CreateFlags);
	}
	Ref<Image> Image::Builder::Acquire()
	{
		ThrowIfInvalidArguments();
		return Image::Acquire(m_Extent, m_ImageType, m_Format, m_Tiling, m_Usage,
			m_MemoryProperties, m_MipLevels, m_ImageAspect, m_ArrayLayers, m_CreateFlags);
	}
	void Image::Builder::ThrowIfInvalidArguments() const
	{
		RAY_MEXPECTS(m_Extent != UInt2(-1, -1), "[Image::Builder] Image extent not specified");
		RAY_MEXPECTS(m_Format != VK_FORMAT_MAX_ENUM, "[Image::Builder] Image format not specified");
		RAY_MEXPECTS(m_Usage != VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM,
			"[Image::Builder] Image usage not specified");
		RAY_MEXPECTS(m_MemoryProperties != VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM,
			"[Image::Builder] Image memory properties not specified");
	}
}  // namespace At0::Ray
