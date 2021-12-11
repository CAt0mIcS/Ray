#pragma once

#include "RTextureSampler.h"
#include "RImage2D.h"

#include <string>


namespace At0::Ray
{
	class RAY_EXPORT Texture2D : public Image2D
	{
	public:
		static Ref<Texture2D> Acquire(std::string_view filepath);
		static Ref<Texture2D> Acquire(UInt2 extent, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags memProps,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT);

		Texture2D(std::string_view filepath);
		Texture2D(UInt2 extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags memProps,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT);

		const TextureSampler& GetSampler() const { return m_Sampler; }

		RAY_DEBUG_FLAG(std::string_view GetPath() const { return m_FilePath; })

	private:
		TextureSampler m_Sampler;

		RAY_DEBUG_FLAG(std::string m_FilePath);
	};

}  // namespace At0::Ray
