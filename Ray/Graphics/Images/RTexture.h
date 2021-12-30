#pragma once

#include "RTextureSampler.h"
#include "RImage.h"

#include <string>


namespace At0::Ray
{
	class RAY_EXPORT Texture : public Image
	{
	public:
		Texture(UInt2 extent, VkImageType imageType, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags memProps,
			Scope<TextureSampler> sampler = TextureSampler::Builder().BuildScoped(),
			uint32_t mipLevels = 1, VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT,
			uint32_t arrayLayers = 1, VkImageCreateFlags createFlags = 0);

		static Ref<Texture> Acquire(UInt2 extent, VkImageType imageType, VkFormat format,
			VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memProps,
			Scope<TextureSampler> sampler = TextureSampler::Builder().BuildScoped(),
			uint32_t mipLevels = 1, VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT,
			uint32_t arrayLayers = 1, VkImageCreateFlags createFlags = 0);

		static Ref<Texture> Acquire(std::string_view filepath);
		Texture(std::string_view filepath);

		const TextureSampler& GetSampler() const { return *m_Sampler; }

		RAY_DEBUG_FLAG(std::string_view GetPath() const { return m_FilePath; })

	private:
		Scope<TextureSampler> m_Sampler = TextureSampler::Builder().BuildScoped();
		RAY_DEBUG_FLAG(std::string m_FilePath);

	public:
		class RAY_EXPORT Builder
		{
		public:
			Builder() = default;

			Builder& SetExtent(UInt2 extent);
			Builder& SetImageType(VkImageType imageType);
			Builder& SetFormat(VkFormat format);
			Builder& SetImageTiling(VkImageTiling imageTiling);
			Builder& SetImageUsage(VkImageUsageFlags imageUsage);
			Builder& SetMemoryProperties(VkMemoryPropertyFlags memoryProperties);
			Builder& SetMipLevels(uint32_t mipLevels);
			Builder& SetImageAspect(VkImageAspectFlags imageAspect);
			Builder& SetArrayLevels(uint32_t arrayLevels);
			Builder& SetImageCreateFlags(VkImageCreateFlags createFlags);
			Builder& SetTextureSampler(Scope<TextureSampler> sampler);

			Ref<Texture> Build();
			Ref<Texture> Acquire();

		private:
			void ThrowIfInvalidArguments() const;

		private:
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

			Scope<TextureSampler> m_Sampler;
		};
	};

}  // namespace At0::Ray
