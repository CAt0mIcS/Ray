#include "Rpch.h"
#include "RImage.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RLogicalDevice.h"

#include "Utils/RException.h"


namespace At0::Ray
{
	Image::Image(UInt2 extent, VkImageType imageType, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags memProps)
		: m_Format(format)
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
			"[Image] Failed to create.");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(Graphics::Get().GetDevice(), m_Image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Graphics::Get().GetPhysicalDevice().FindMemoryType(
			memRequirements.memoryTypeBits, memProps);

		RAY_VK_THROW_FAILED(
			vkAllocateMemory(Graphics::Get().GetDevice(), &allocInfo, nullptr, &m_ImageMemory),
			"[Image] Failed to allocate image memory.");

		vkBindImageMemory(Graphics::Get().GetDevice(), m_Image, m_ImageMemory, 0);

		m_ImageView = MakeScope<ImageView>(m_Image, format, VK_IMAGE_ASPECT_DEPTH_BIT);
	}

	Image::~Image()
	{
		vkDestroyImage(Graphics::Get().GetDevice(), m_Image, nullptr);
		vkFreeMemory(Graphics::Get().GetDevice(), m_ImageMemory, nullptr);
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
