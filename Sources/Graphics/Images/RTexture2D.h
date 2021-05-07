#pragma once

#include "RTexture.h"
#include "RTextureSampler.h"

#include <string_view>


namespace At0::Ray
{
	class Image2D;

	class RAY_EXPORT Texture2D : public Texture
	{
	public:
		Texture2D(std::string_view filepath);
		Texture2D(UInt2 extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags memProps,
			VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT);

		Texture2D& operator=(Texture2D&& other);
		Texture2D(Texture2D&& other);

		const TextureSampler& GetSampler() const { return m_Sampler; }
		const Image2D& GetImage() const { return *m_Image; }
		Image2D& GetImage() { return *m_Image; }

	private:
		Scope<Image2D> m_Image;
		TextureSampler m_Sampler;
	};

}  // namespace At0::Ray
