#pragma once

#include "../../RBase.h"
#include "../../Core/RMath.h"
#include "../../Utils/RNonCopyable.h"
#include "RImageView.h"

#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>


namespace At0::Ray
{
	class Buffer;

	class Image : NonCopyable
	{
	public:
		Image(UInt2 extent, VkImageType imageType, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags memProps, uint32_t mipLevels = 1,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT, uint32_t arrayLayers = 1,
			VkImageCreateFlags createFlags = 0);
		virtual ~Image();

		operator const VkImage&() const { return m_Image; }
		const ImageView& GetImageView() const { return *m_ImageView; }
		VkImageLayout GetImageLayout() const { return m_ImageLayout; }
		UInt2 GetExtent() const { return m_Extent; }
		VkImageType GetImageType() const { return m_ImageType; }
		VkFormat GetFormat() const { return m_Format; }
		VkImageTiling GetTiling() const { return m_Tiling; }
		VkImageUsageFlags GetUsageFlags() const { return m_Usage; }
		VkMemoryPropertyFlags GetMemoryProperties() const { return m_MemoryProperties; }
		uint32_t GetMipLevels() const { return m_MipLevels; }
		VkImageAspectFlags GetAspectFlags() const { return m_ImageAspect; }
		uint32_t GetArrayLayers() const { return m_ArrayLayers; }

		void TransitionLayout(VkImageLayout newLayout);
		void CopyFromBuffer(const Buffer& buffer, std::vector<VkBufferImageCopy> copyRegions = {});
		bool GenerateMipmaps();

		Image& operator=(Image&& other) noexcept;
		Image(Image&& other) noexcept { *this = std::move(other); }

		static void TransitionLayout(VkImage image, VkImageLayout oldLayout,
			VkImageLayout newLayout, uint32_t mipLevels = 1, uint32_t layerCount = 1);
		static bool GenerateMipmaps(
			VkImage image, VkFormat imageFormat, int32_t width, int32_t height, uint32_t mipLevels);
		static std::vector<VkFormat> FindSupportedFormats(std::vector<VkFormat> candidates,
			VkImageTiling tiling, VkFormatFeatureFlags featureFlags);

	protected:
		void Setup();
		Image() = default;

	protected:
		VkImage m_Image = VK_NULL_HANDLE;
		VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
		Scope<ImageView> m_ImageView;

		VkImageLayout m_ImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		UInt2 m_Extent{};
		VkImageType m_ImageType;
		VkFormat m_Format = VK_FORMAT_UNDEFINED;
		VkImageTiling m_Tiling;
		VkImageUsageFlags m_Usage;
		VkMemoryPropertyFlags m_MemoryProperties;
		uint32_t m_MipLevels = 1;
		VkImageAspectFlags m_ImageAspect;
		uint32_t m_ArrayLayers = 1;
		VkImageCreateFlags m_CreateFlags = 0;
	};
}  // namespace At0::Ray
