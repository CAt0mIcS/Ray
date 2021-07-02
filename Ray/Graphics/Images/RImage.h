#pragma once

#include "Ray/RBase.h"
#include "Ray/Core/RMath.h"
#include "Ray/Core/RResource.h"
#include "Ray/Utils/RNonCopyable.h"
#include "RImageView.h"

#include <vulkan/vulkan_core.h>
#include <RayRenderer/Core/RImage.h>
#include <RayRenderer/Core/RPhysicalDevice.h>
#include <vector>
#include <string>


namespace At0::Ray
{
	class Buffer;

	class Image : public Resource, NonCopyable
	{
	public:
		static Ref<Image> Acquire(UInt2 extent, RrImageType imageType, RrFormat format,
			RrImageTiling tiling, RrImageUsageFlags usage, RrMemoryPropertyFlags memProps,
			uint32_t mipLevels = 1, RrImageAspectFlags imageAspect = RrImageAspectColor,
			uint32_t arrayLayers = 1, RrImageCreateFlags createFlags = 0);

		Image(UInt2 extent, RrImageType imageType, RrFormat format, RrImageTiling tiling,
			RrImageUsageFlags usage, RrMemoryPropertyFlags memProps, uint32_t mipLevels = 1,
			RrImageAspectFlags imageAspect = RrImageAspectColor, uint32_t arrayLayers = 1,
			RrImageCreateFlags createFlags = 0);
		virtual ~Image();

		operator VkImage() const { return (VkImage)m_Image; }
		operator RrImage() const { return m_Image; }
		const RrDeviceMemory& GetImageMemory() const { return m_ImageMemory; }
		const ImageView& GetImageView() const { return *m_ImageView; }
		RrImageLayout GetImageLayout() const { return m_ImageLayout; }
		UInt2 GetExtent() const { return m_Extent; }
		RrImageType GetImageType() const { return m_ImageType; }
		RrFormat GetFormat() const { return m_Format; }
		RrImageTiling GetTiling() const { return m_Tiling; }
		RrImageUsageFlags GetUsageFlags() const { return m_Usage; }
		RrMemoryPropertyFlags GetMemoryProperties() const { return m_MemoryProperties; }
		uint32_t GetMipLevels() const { return m_MipLevels; }
		RrImageAspectFlags GetAspectFlags() const { return m_ImageAspect; }
		uint32_t GetArrayLayers() const { return m_ArrayLayers; }

		void TransitionLayout(RrImageLayout newLayout);
		void CopyFromBuffer(const Buffer& buffer, std::vector<VkBufferImageCopy> copyRegions = {});
		Buffer&& CopyToBuffer(std::vector<VkBufferImageCopy> copyRegions = {});
		bool GenerateMipmaps();
		void WritePPM(std::string_view filepath);

		Image& operator=(Image&& other) noexcept;
		Image(Image&& other) noexcept { *this = std::move(other); }

		static void TransitionLayout(RrImage image, RrImageLayout oldLayout,
			RrImageLayout newLayout, uint32_t mipLevels = 1, uint32_t layerCount = 1);
		static bool GenerateMipmaps(
			RrImage image, RrFormat imageFormat, int32_t width, int32_t height, uint32_t mipLevels);
		static std::vector<RrFormat> FindSupportedFormats(std::vector<RrFormat> candidates,
			RrImageTiling tiling, RrFormatFeatureFlags featureFlags);
		static Buffer CopyToBuffer(RrImage image);

	protected:
		void Setup();
		Image() = default;

	protected:
		RrImage m_Image = nullptr;
		RrDeviceMemory m_ImageMemory = nullptr;
		Scope<ImageView> m_ImageView;

		RrImageLayout m_ImageLayout = RrImageLayoutUndefined;
		UInt2 m_Extent{};
		RrImageType m_ImageType;
		RrFormat m_Format = RRFORMAT_UNDEFINED;
		RrImageTiling m_Tiling;
		RrImageUsageFlags m_Usage;
		RrMemoryPropertyFlags m_MemoryProperties;
		uint32_t m_MipLevels = 1;
		RrImageAspectFlags m_ImageAspect;
		uint32_t m_ArrayLayers = 1;
		RrImageCreateFlags m_CreateFlags = 0;
	};
}  // namespace At0::Ray
