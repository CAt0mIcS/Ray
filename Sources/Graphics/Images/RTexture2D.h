#pragma once

#include "RTextureSampler.h"
#include "RImage2D.h"

#include <string_view>


namespace At0::Ray
{
	class RAY_EXPORT Texture2D : public Image2D
	{
	public:
		Texture2D(std::string_view filepath);
		Texture2D(UInt2 extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags memProps,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT);

		const TextureSampler& GetSampler() const { return m_Sampler; }

	private:
		TextureSampler m_Sampler;
	};

}  // namespace At0::Ray
