#pragma once

#include "../../RBase.h"
#include "../../Core/RMath.h"
#include "../../Core/RResourceManager.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>

#include <vector>
#include <string_view>


namespace At0::Ray
{
	class Buffer;
	class ImageView;

	class RAY_EXPORT Image : public Resource, protected NonCopyable
	{
	public:
		static Ref<Image> Acquire(UInt2 extent, VkImageType imageType, VkFormat format,
			VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memProps,
			uint32_t mipLevels = 1, VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT,
			uint32_t arrayLayers = 1, VkImageCreateFlags createFlags = 0);

		Image(UInt2 extent, VkImageType imageType, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags memProps, uint32_t mipLevels = 1,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT, uint32_t arrayLayers = 1,
			VkImageCreateFlags createFlags = 0);
		virtual ~Image();

		operator const VkImage&() const { return m_Image; }
		const VkDeviceMemory& GetImageMemory() const { return m_ImageMemory; }
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
		void CopyFromData(const std::vector<uint8_t>& data);
		void CopyFromData(const void* data, uint32_t size);
		Buffer&& CopyToBuffer(std::vector<VkBufferImageCopy> copyRegions = {});
		bool GenerateMipmaps();
		void WritePPM(std::string_view filepath);

		Image& operator=(Image&& other) noexcept;
		Image(Image&& other) noexcept;

		static void TransitionLayout(VkImage image, VkImageLayout oldLayout,
			VkImageLayout newLayout, uint32_t mipLevels = 1, uint32_t layerCount = 1);
		static bool GenerateMipmaps(
			VkImage image, VkFormat imageFormat, int32_t width, int32_t height, uint32_t mipLevels);
		static std::vector<VkFormat> FindSupportedFormats(std::vector<VkFormat> candidates,
			VkImageTiling tiling, VkFormatFeatureFlags featureFlags);

	protected:
		void Setup();
		Image();

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

	public:
		template<typename B>
		class RAY_EXPORT BuilderBase
		{
		public:
			BuilderBase() = default;
			virtual ~BuilderBase() = default;

			B& SetExtent(UInt2 extent)
			{
				m_Extent = extent;
				return *(B*)this;
			}
			B& SetImageType(VkImageType imageType)
			{
				m_ImageType = imageType;
				return *(B*)this;
			}
			B& SetFormat(VkFormat format)
			{
				m_Format = format;
				return *(B*)this;
			}
			B& SetImageTiling(VkImageTiling imageTiling)
			{
				m_Tiling = imageTiling;
				return *(B*)this;
			}
			B& SetImageUsage(VkImageUsageFlags imageUsage)
			{
				m_Usage = imageUsage;
				return *(B*)this;
			}
			B& SetMemoryProperties(VkMemoryPropertyFlags memoryProperties)
			{
				m_MemoryProperties = memoryProperties;
				return *(B*)this;
			}
			B& SetMipLevels(uint32_t mipLevels)
			{
				m_MipLevels = mipLevels;
				return *(B*)this;
			}
			B& SetImageAspect(VkImageAspectFlags imageAspect)
			{
				m_ImageAspect = imageAspect;
				return *(B*)this;
			}
			B& SetArrayLevels(uint32_t arrayLevels)
			{
				m_ArrayLayers = arrayLevels;
				return *(B*)this;
			}
			B& SetImageCreateFlags(VkImageCreateFlags createFlags)
			{
				m_CreateFlags = createFlags;
				return *(B*)this;
			}
			B& SetData(void* data, uint32_t size)
			{
				m_Data.clear();
				m_Data.resize(size);
				for (int i = 0; i < size; ++i)
					m_Data[i] = *((uint8_t*)data + i);
				return *(B*)this;
			}
			B& SetData(std::vector<uint8_t> data)
			{
				m_Data = std::move(data);
				return *(B*)this;
			}

		protected:
			UInt2 m_Extent{ -1, -1 };
			VkFormat m_Format = VK_FORMAT_MAX_ENUM;
			VkImageUsageFlags m_Usage = VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM;
			VkMemoryPropertyFlags m_MemoryProperties = VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM;

			VkImageTiling m_Tiling = VK_IMAGE_TILING_OPTIMAL;
			VkImageType m_ImageType = VK_IMAGE_TYPE_2D;
			uint32_t m_MipLevels = 1;
			VkImageAspectFlags m_ImageAspect = VK_IMAGE_ASPECT_COLOR_BIT;
			uint32_t m_ArrayLayers = 1;
			VkImageCreateFlags m_CreateFlags = 0;
			std::vector<uint8_t> m_Data;
		};

		class RAY_EXPORT Builder : public BuilderBase<Builder>
		{
		public:
			Ref<Image> Build();
			Ref<Image> Acquire();

		private:
			void ThrowIfInvalidArguments() const;
		};
	};
}  // namespace At0::Ray
