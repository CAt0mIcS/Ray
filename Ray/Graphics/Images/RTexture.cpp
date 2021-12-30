#include "RTexture.h"

#include "../Buffers/RBuffer.h"

#ifdef STB_IMAGE_IMPLEMENTATION
	#undef STB_IMAGE_IMPLEMENTATION
#else
	#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb/stb_image.h>


namespace At0::Ray
{
	Ref<Texture> Texture::Acquire(std::string_view filepath)
	{
		return Resources::Get().EmplaceIfNonExistent<Texture>(filepath.data(), filepath);
	}

	Ref<Texture> Texture::Acquire(UInt2 extent, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags memProps, VkImageAspectFlags imageAspect,
		VkImageType imageType)
	{
		return Resources::Get().EmplaceIfNonExistent<Texture>(
			String::Serialize("Texture{0}{1}{2}{3}{4}{5}{6}{7}", extent.x, extent.y,
				(uint32_t)format, (uint32_t)tiling, (uint32_t)usage, (uint32_t)memProps,
				(uint32_t)imageAspect, (uint32_t)imageType),
			std::move(extent), format, tiling, usage, memProps, imageAspect, imageType);
	}

	Texture::Texture(std::string_view filepath) RAY_DEBUG_FLAG( : m_FilePath(filepath))
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels =
			stbi_load(filepath.data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = (VkDeviceSize)texWidth * texHeight * 4;

		if (!pixels)
			ThrowRuntime("[Texture] Failed to load texture from file \"{0}\"", filepath);

		// RAY_TODO: Option to enable/disable mipmapping
		uint32_t mipLevels = (uint32_t)(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

		Buffer stagingBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pixels);

		stbi_image_free(pixels);

		m_Extent = UInt2{ texWidth, texHeight };
		m_ImageType = VK_IMAGE_TYPE_2D;
		m_Format = VK_FORMAT_R8G8B8A8_SRGB;
		m_Tiling = VK_IMAGE_TILING_OPTIMAL;
		m_Usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
				  VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		m_MemoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		m_ImageAspect = VK_IMAGE_ASPECT_COLOR_BIT;
		m_MipLevels = mipLevels;
		m_ArrayLayers = 1;
		m_CreateFlags = 0;
		Image::Setup();

		TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyFromBuffer(stagingBuffer);

		// If mitpmap generation failed we need to transition the layout ourselves
		if (!GenerateMipmaps())
			TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	Texture::Texture(UInt2 extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags memProps, VkImageAspectFlags imageAspect, VkImageType imageType)
		: Image(extent, imageType, format, tiling, usage, memProps, 1, imageAspect)
	{
	}
}  // namespace At0::Ray
