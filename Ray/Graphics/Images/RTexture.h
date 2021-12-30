#pragma once

#include "RTextureSampler.h"
#include "RImage.h"

#include <string>


namespace At0::Ray
{
	class RAY_EXPORT Texture : public Image
	{
	public:
		static Ref<Texture> Acquire(std::string_view filepath);
		static Ref<Texture> Acquire(UInt2 extent, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags memProps,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT,
			VkImageType imageType = VK_IMAGE_TYPE_2D);

		Texture(std::string_view filepath);
		Texture(UInt2 extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags memProps,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT,
			VkImageType imageType = VK_IMAGE_TYPE_2D);

		const TextureSampler& GetSampler() const { return m_Sampler; }

		RAY_DEBUG_FLAG(std::string_view GetPath() const { return m_FilePath; })

	private:
		TextureSampler m_Sampler;

		RAY_DEBUG_FLAG(std::string m_FilePath);
	};

}  // namespace At0::Ray
