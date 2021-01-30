#include "Rpch.h"
#include "RImage.h"

#include "Debug/RException.h"
#include "Graphics/RGraphics.h"
#include "Devices/Vulkan/RLogicalDevice.h"


namespace At0::Ray
{
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
}  // namespace At0::Ray
