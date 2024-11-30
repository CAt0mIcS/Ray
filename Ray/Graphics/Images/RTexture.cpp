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
	Texture::Texture(RenderContext& context, Ref<CommandPool> transientCommandPool, UInt2 extent,
		VkImageType imageType, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags memProps, Scope<TextureSampler> sampler, uint32_t mipLevels,
		VkImageAspectFlags imageAspect, uint32_t arrayLayers, VkImageCreateFlags createFlags)
		: Image(context, std::move(transientCommandPool), extent, imageType, format, tiling, usage,
			  memProps, mipLevels, imageAspect, arrayLayers, createFlags),
		  m_Sampler(std::move(sampler))
	{
	}

	Texture::Texture(
		RenderContext& context, Ref<CommandPool> transientCommandPool, std::string_view filepath)
		: Image(context, std::move(transientCommandPool)) RAY_DEBUG_FLAG(, m_FilePath(filepath))
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels =
			stbi_load(filepath.data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = (VkDeviceSize)texWidth * texHeight * 4;

		if (!pixels)
			ThrowRuntime("[Texture] Failed to load texture from file \"{0}\"", filepath);

		// RAY_TODO: Option to enable/disable mipmapping
		uint32_t mipLevels = (uint32_t)(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

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

		CopyFromData(pixels, imageSize);

		stbi_image_free(pixels);

		// If mitpmap generation failed we need to transition the layout ourselves
		if (!GenerateMipmaps())
			TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////// BUILDER //////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Texture::Builder::Builder(RenderContext& context, Ref<CommandPool> transientCommandPool)
		: Image::BuilderBase<Texture::Builder>(context, std::move(transientCommandPool))
	{
	}

	Texture::Builder& Texture::Builder::SetTextureSampler(Scope<TextureSampler> sampler)
	{
		m_Sampler = std::move(sampler);
		return *this;
	}

	Ref<Texture> Texture::Builder::Build()
	{
		ThrowIfInvalidArguments();

		// Default sampler
		if (!m_Sampler)
			m_Sampler = TextureSampler::Builder(*m_Context).BuildScoped();

		auto texture = MakeRef<Texture>(*m_Context, std::move(m_TransientCommandPool), m_Extent,
			m_ImageType, m_Format, m_Tiling, m_Usage, m_MemoryProperties, std::move(m_Sampler),
			m_MipLevels, m_ImageAspect, m_ArrayLayers, m_CreateFlags);
		if (!m_Data.empty())
			texture->CopyFromData(m_Data);
		return texture;
	}

	void Texture::Builder::ThrowIfInvalidArguments() const
	{
		RAY_MEXPECTS(m_Extent != UInt2(-1, -1), "[Image::Builder] Image extent not specified");
		RAY_MEXPECTS(m_Format != VK_FORMAT_MAX_ENUM, "[Image::Builder] Image format not specified");
		RAY_MEXPECTS(m_Usage != VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM,
			"[Image::Builder] Image usage not specified");
		RAY_MEXPECTS(m_MemoryProperties != VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM,
			"[Image::Builder] Image memory properties not specified");
	}
}  // namespace At0::Ray
