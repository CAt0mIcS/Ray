#pragma once

#include "../../RBase.h"
#include "../../Core/RMath.h"
#include "RImageView.h"

#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>


namespace At0::Ray
{
	class Buffer;

	class Image
	{
	public:
		Image(UInt2 extent, VkImageType imageType, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags memProps,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT);
		virtual ~Image();

		VkFormat GetFormat() const { return m_Format; }
		operator const VkImage&() const { return m_Image; }
		const VkImageView& GetImageView() const { return *m_ImageView; }

		void TransitionLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
		static void TransitionLayout(
			VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
		void CopyFromBuffer(const Buffer& buffer);

		static std::vector<VkFormat> FindSupportedFormats(std::vector<VkFormat> candidates,
			VkImageTiling tiling, VkFormatFeatureFlags featureFlags);

	protected:
		VkImage m_Image = VK_NULL_HANDLE;
		UInt2 m_Extent{};
		VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
		Scope<ImageView> m_ImageView;

		VkFormat m_Format;
	};
}  // namespace At0::Ray
