#include "Rpch.h"
#include "RTexture2D.h"

#include "RImage2D.h"

#include "Ray/Utils/RException.h"
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

	Ref<Texture2D> Texture2D::Acquire(UInt2 extent, RrFormat format, RrImageTiling tiling,
		RrImageUsageFlags usage, RrMemoryPropertyFlags memProps, RrImageAspectFlags imageAspect)
	{
		return Resources::Get().EmplaceIfNonExistent<Texture2D>(
			String::Serialize("Texture2D{0}{1}{2}{3}{4}{5}{6}", extent.x, extent.y,
				(uint32_t)format, (uint32_t)tiling, (uint32_t)usage, (uint32_t)memProps,
				(uint32_t)imageAspect),
			std::move(extent), format, tiling, usage, memProps, imageAspect);
	}

	Texture2D::Texture2D(std::string_view filepath)
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels =
			stbi_load(filepath.data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		RrDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels)
			ThrowRuntime("[Texture2D] Failed to load texture from file \"{0}\"", filepath);

		// RAY_TODO: Option to enable/disable mipmapping
		uint32_t mipLevels = (uint32_t)(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

		Buffer stagingBuffer(imageSize, RrBufferUsageTransferSrc,
			RrMemoryPropertyHostVisible | RrMemoryPropertyHostCoherent, pixels);

		stbi_image_free(pixels);

		Image2D::Setup(UInt2{ texWidth, texHeight }, RRFORMAT_R8G8B8A8_SRGB, RrImageTilingOptimal,
			RrImageUsageTransferDst | RrImageUsageSampled | RrImageUsageTransferSrc,
			RrSharingModeExclusive, mipLevels);

		TransitionLayout(RrImageLayoutTransferDst);
		CopyFromBuffer(stagingBuffer);

		// If mitpmap generation failed we need to transition the layout ourselves
		if (!GenerateMipmaps())
			TransitionLayout(RrImageLayoutShaderReadOnly);
	}

	Texture2D::Texture2D(UInt2 extent, RrFormat format, RrImageTiling tiling,
		RrImageUsageFlags usage, RrMemoryPropertyFlags memProps, RrImageAspectFlags imageAspect)
		: Image2D(extent, format, tiling, usage, memProps, imageAspect)
	{
	}
}  // namespace At0::Ray
