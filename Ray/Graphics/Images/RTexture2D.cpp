#include "Rpch.h"
#include "RTexture2D.h"

#include "RImage2D.h"

#include "Utils/RException.h"
#include "../Buffers/RBuffer.h"

#ifdef STB_IMAGE_IMPLEMENTATION
	#undef STB_IMAGE_IMPLEMENTATION
#else
	#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb/stb_image.h>


namespace At0::Ray
{
	Ref<Texture2D> Texture2D::Acquire(std::string_view filepath)
	{
		return Resources::Get().EmplaceIfNonExistent<Texture2D>(filepath.data(), filepath);
	}

	Ref<Texture2D> Texture2D::Acquire(UInt2 extent, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags memProps, VkImageAspectFlags imageAspect)
	{
		return Resources::Get().EmplaceIfNonExistent<Texture2D>(
			String::Serialize("Texture2D{0}{1}{2}{3}{4}{5}{6}", extent.x, extent.y,
				(uint32_t)format, (uint32_t)tiling, (uint32_t)usage, (uint32_t)memProps,
				(uint32_t)imageAspect),
			std::move(extent), format, tiling, usage, memProps, imageAspect);
	}

	Texture2D::Texture2D(std::string_view filepath) : RAY_DEBUG_FLAG(m_FilePath(filepath))
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels =
			stbi_load(filepath.data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels)
			ThrowRuntime("[Texture2D] Failed to load texture from file \"{0}\"", filepath);

		// RAY_TODO: Option to enable/disable mipmapping
		uint32_t mipLevels = (uint32_t)(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

		Buffer stagingBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pixels);

		stbi_image_free(pixels);

		Image2D::Setup(UInt2{ texWidth, texHeight }, VK_FORMAT_R8G8B8A8_SRGB,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
				VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
			VK_SHARING_MODE_EXCLUSIVE, mipLevels);

		TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyFromBuffer(stagingBuffer);

		// If mitpmap generation failed we need to transition the layout ourselves
		if (!GenerateMipmaps())
			TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	Texture2D::Texture2D(UInt2 extent, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags memProps, VkImageAspectFlags imageAspect)
		: Image2D(extent, format, tiling, usage, memProps, imageAspect)
	{
	}
}  // namespace At0::Ray
