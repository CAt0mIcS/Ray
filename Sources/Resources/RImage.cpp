#include "Rpch.h"
#include "RImage.h"

#include "Debug/RException.h"
#include "Graphics/RGraphics.h"
#include "Devices/Vulkan/RLogicalDevice.h"
#include "Devices/Vulkan/RPhysicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"


namespace At0::Ray
{
	static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags requiredProperties)
	{
		const PhysicalDevice& physicalDevice = Graphics::Get().GetPhysicalDevice();
		auto memoryProperties = physicalDevice.GetMemoryProperties();

		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			uint32_t memoryTypeBits = 1 << i;

			if (typeFilter & memoryTypeBits && (memoryProperties.memoryTypes[i].propertyFlags &
												   requiredProperties) == requiredProperties)
			{
				return i;
			}
		}

		RAY_THROW_RUNTIME("[FindMemoryType] Failed to find valid memory type for buffer");
	}

	Image::Image(VkFilter filter, VkSamplerAddressMode addressMode, VkSampleCountFlagBits samples,
		VkImageLayout layout, VkImageUsageFlags usage, VkFormat format, uint32_t mipLevels,
		uint32_t arrayLayers, const VkExtent3D& extent)
	{
	}

	WriteDescriptorSet Image::GetWriteDescriptor(uint32_t binding, VkDescriptorType descriptorType,
		const std::optional<OffsetSize>& offsetSize) const
	{
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.sampler = m_Sampler;
		imageInfo.imageView = m_View;
		imageInfo.imageLayout = m_Layout;

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = VK_NULL_HANDLE;  // Will be set in the descriptor handler.
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.descriptorType = descriptorType;
		// descriptorWrite.pImageInfo = &imageInfo;
		return { descriptorWrite, imageInfo };
	}

	void Image::CreateImageView(const VkImage_T* image, VkImageView_T*& imageView,
		VkImageViewType type, VkFormat format, uint32_t imageAspect, uint32_t mipLevels,
		uint32_t baseMipLevel, uint32_t layerCount, uint32_t baseArrayLayer)
	{
		const LogicalDevice& logicalDevice = Graphics::Get().GetLogicalDevice();

		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = (VkImage)image;
		createInfo.viewType = type;
		createInfo.format = format;
		createInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G,
			VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		createInfo.subresourceRange.aspectMask = imageAspect;
		createInfo.subresourceRange.baseMipLevel = baseMipLevel;
		createInfo.subresourceRange.levelCount = mipLevels;
		createInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
		createInfo.subresourceRange.layerCount = layerCount;

		RAY_VK_THROW_FAILED(vkCreateImageView(logicalDevice, &createInfo, nullptr, &imageView),
			"[Image] Failed to create image view.");
	}
	uint32_t Image::GetMipLevels(const VkExtent3D& extent)
	{
		return static_cast<uint32_t>(
			std::floor(std::log2(std::max(extent.width, std::max(extent.height, extent.depth)))) +
			1);
	}
	void Image::CreateImage(VkImage& image, VkDeviceMemory& memory, const VkExtent3D& extent,
		VkFormat format, VkSampleCountFlagBits samples, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t mipLevels,
		uint32_t arrayLayers, VkImageType type)
	{
		const LogicalDevice& logicalDevice = Graphics::Get().GetLogicalDevice();

		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.flags = arrayLayers == 6 ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
		imageCreateInfo.imageType = type;
		imageCreateInfo.format = format;
		imageCreateInfo.extent = extent;
		imageCreateInfo.mipLevels = mipLevels;
		imageCreateInfo.arrayLayers = arrayLayers;
		imageCreateInfo.samples = samples;
		imageCreateInfo.tiling = tiling;
		imageCreateInfo.usage = usage;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		RAY_VK_THROW_FAILED(vkCreateImage(logicalDevice, &imageCreateInfo, nullptr, &image),
			"[Image] Failed to create");

		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(logicalDevice, image, &memoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex =
			FindMemoryType(memoryRequirements.memoryTypeBits, properties);
		RAY_VK_THROW_FAILED(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &memory),
			"[Image] Failed to allocate memory.");

		RAY_VK_THROW_FAILED(vkBindImageMemory(logicalDevice, image, memory, 0),
			"[Image] Failed to bind memory to image.");
	}

	void Image::CreateImageSampler(VkSampler& sampler, VkFilter filter,
		VkSamplerAddressMode addressMode, bool anisotropic, uint32_t mipLevels)
	{
		const auto& logicalDevice = Graphics::Get().GetLogicalDevice();
		const auto& physicalDevice = Graphics::Get().GetPhysicalDevice();

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.magFilter = filter;
		samplerCreateInfo.minFilter = filter;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCreateInfo.addressModeU = addressMode;
		samplerCreateInfo.addressModeV = addressMode;
		samplerCreateInfo.addressModeW = addressMode;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.anisotropyEnable = static_cast<VkBool32>(anisotropic);
		samplerCreateInfo.maxAnisotropy =
			(anisotropic && logicalDevice.GetEnabledFeatures().samplerAnisotropy) ?
				std::min(16.0f, physicalDevice.GetProperties().limits.maxSamplerAnisotropy) :
				1.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = static_cast<float>(mipLevels);
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
		RAY_VK_THROW_FAILED(vkCreateSampler(logicalDevice, &samplerCreateInfo, nullptr, &sampler),
			"[Image] Failed to create sampler.");
	}

	void Image::TransitionImageLayout(const VkImage& image, VkFormat format,
		VkImageLayout srcImageLayout, VkImageLayout dstImageLayout, VkImageAspectFlags imageAspect,
		uint32_t mipLevels, uint32_t baseMipLevel, uint32_t layerCount, uint32_t baseArrayLayer)
	{
		CommandBuffer commandBuffer;

		VkImageMemoryBarrier imageMemoryBarrier = {};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.oldLayout = srcImageLayout;
		imageMemoryBarrier.newLayout = dstImageLayout;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange.aspectMask = imageAspect;
		imageMemoryBarrier.subresourceRange.baseMipLevel = baseMipLevel;
		imageMemoryBarrier.subresourceRange.levelCount = mipLevels;
		imageMemoryBarrier.subresourceRange.baseArrayLayer = baseArrayLayer;
		imageMemoryBarrier.subresourceRange.layerCount = layerCount;

		// Source access mask controls actions that have to be finished on the old layout before it
		// will be transitioned to the new layout.
		switch (srcImageLayout)
		{
		case VK_IMAGE_LAYOUT_UNDEFINED: imageMemoryBarrier.srcAccessMask = 0; break;
		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		default:
			// throw std::runtime_error("Unsupported image layout transition source");
			break;
		}

		// Destination access mask controls the dependency for the new image layout.
		switch (dstImageLayout)
		{
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			imageMemoryBarrier.dstAccessMask =
				imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			if (imageMemoryBarrier.srcAccessMask == 0)
			{
				imageMemoryBarrier.srcAccessMask =
					VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
			}

			imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		default:
			// throw std::runtime_error("Unsupported image layout transition destination");
			break;
		}

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
			VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

		commandBuffer.SubmitIdle();
	}

	void Image::CreateMipmaps(const VkImage& image, const VkExtent3D& extent, VkFormat format,
		VkImageLayout dstImageLayout, uint32_t mipLevels, uint32_t baseArrayLayer,
		uint32_t layerCount)
	{
		const PhysicalDevice& physicalDevice = Graphics::Get().GetPhysicalDevice();

		// Get device properites for the requested Image format.
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

		// Mip-chain generation requires support for blit source and destination
		assert(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT);
		assert(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT);

		CommandBuffer commandBuffer;

		for (uint32_t i = 1; i < mipLevels; i++)
		{
			VkImageMemoryBarrier barrier0 = {};
			barrier0.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier0.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier0.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier0.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier0.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier0.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier0.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier0.image = image;
			barrier0.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier0.subresourceRange.baseMipLevel = i - 1;
			barrier0.subresourceRange.levelCount = 1;
			barrier0.subresourceRange.baseArrayLayer = baseArrayLayer;
			barrier0.subresourceRange.layerCount = layerCount;
			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier0);

			VkImageBlit imageBlit = {};
			imageBlit.srcOffsets[1] = { int32_t(extent.width >> (i - 1)),
				int32_t(extent.height >> (i - 1)), 1 };
			imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.srcSubresource.mipLevel = i - 1;
			imageBlit.srcSubresource.baseArrayLayer = baseArrayLayer;
			imageBlit.srcSubresource.layerCount = layerCount;
			imageBlit.dstOffsets[1] = { int32_t(extent.width >> i), int32_t(extent.height >> i),
				1 };
			imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.dstSubresource.mipLevel = i;
			imageBlit.dstSubresource.baseArrayLayer = baseArrayLayer;
			imageBlit.dstSubresource.layerCount = layerCount;
			vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);

			VkImageMemoryBarrier barrier1 = {};
			barrier1.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier1.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier1.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			barrier1.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier1.newLayout = dstImageLayout;
			barrier1.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier1.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier1.image = image;
			barrier1.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier1.subresourceRange.baseMipLevel = i - 1;
			barrier1.subresourceRange.levelCount = 1;
			barrier1.subresourceRange.baseArrayLayer = baseArrayLayer;
			barrier1.subresourceRange.layerCount = layerCount;
			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier1);
		}

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = dstImageLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = baseArrayLayer;
		barrier.subresourceRange.layerCount = layerCount;
		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		commandBuffer.SubmitIdle();
	}

	VkFormat Image::FindSupportedFormat(const std::vector<VkFormat>& candidates,
		VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		const auto& physicalDevice = Graphics::Get().GetPhysicalDevice();

		for (const auto& format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR &&
				(props.linearTilingFeatures & features) == features)
				return format;
			if (tiling == VK_IMAGE_TILING_OPTIMAL &&
				(props.optimalTilingFeatures & features) == features)
				return format;
		}

		return VK_FORMAT_UNDEFINED;
	}

	bool Image::HasStencil(VkFormat format)
	{
		static const std::vector<VkFormat> STENCIL_FORMATS = { VK_FORMAT_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D32_SFLOAT_S8_UINT };
		return std::find(STENCIL_FORMATS.begin(), STENCIL_FORMATS.end(), format) !=
			   std::end(STENCIL_FORMATS);
	}

}  // namespace At0::Ray
