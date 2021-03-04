#include "Rpch.h"
#include "RTexture2D.h"

#include "Utils/RException.h"
#include "../Buffers/RBuffer.h"

#ifdef STB_IMAGE_IMPLEMENTATION
	#undef STB_IMAGE_IMPLEMENTATION
#else
	#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image/stb_image.h>

namespace At0::Ray
{
	Texture2D::Texture2D(std::string_view filepath)
	{
		Buffer stagingBuffer;
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels =
			stbi_load("textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels)
			RAY_THROW_RUNTIME("[Texture2D] Failed to load texture from file \"{0}\"", filepath);

		stagingBuffer = Buffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pixels);

		stbi_image_free(pixels);

		m_Image = Image2D({ texWidth, texHeight }, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_SHARING_MODE_EXCLUSIVE);

		m_Image.TransitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		m_Image.CopyFromBuffer(stagingBuffer);

		m_Image.TransitionLayout(
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	Texture2D& Texture2D::operator=(Texture2D&& other)
	{
		m_Image = std::move(other.m_Image);
		return *this;
	}

	Texture2D::Texture2D(Texture2D&& other) { *this = std::move(other); }
}  // namespace At0::Ray
