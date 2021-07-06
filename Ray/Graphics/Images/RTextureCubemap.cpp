#include "Rpch.h"
#include "RTextureCubemap.h"

#include "Ray/Utils/RException.h"
#include "RTextureSampler.h"
#include "Ray/Utils/RAssert.h"
#include "../Buffers/RBuffer.h"


namespace At0::Ray
{
	TextureCubemap::TextureCubemap(std::string_view filepath)
	{
		// ktxResult result;
		// ktxTexture* ktxTexture;

		// result = ktxTexture_CreateFromNamedFile(
		// 	filepath.data(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktxTexture);
		// RAY_MEXPECTS(result == KTX_SUCCESS, "[Skybox] Failed to load texture \"{0}\"", filepath);

		// ktx_uint8_t* ktxTextureData = ktxTexture_GetData(ktxTexture);
		// ktx_size_t ktxTextureSize = ktxTexture_GetSize(ktxTexture);

		// Buffer stagingBuffer((RrDeviceSize)ktxTextureSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		// 	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		// 	ktxTextureData);

		// Image2D::Setup(UInt2{ ktxTexture->baseWidth, ktxTexture->baseHeight },
		// 	VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
		// 	VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		// 	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ktxTexture->numLevels, VK_IMAGE_ASPECT_COLOR_BIT,
		// 	6,	// Array layers count as cube faces in vulkan
		// 	VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT);

		// // Setup buffer copy regions for each face including all of its miplevels
		// std::vector<VkBufferImageCopy> bufferCopyRegions;
		// uint32_t offset = 0;

		// for (uint32_t face = 0; face < 6; face++)
		// {
		// 	for (uint32_t level = 0; level < GetMipLevels(); level++)
		// 	{
		// 		// Calculate offset into staging buffer for the current mip level and face
		// 		ktx_size_t offset;
		// 		KTX_error_code ret = ktxTexture_GetImageOffset(ktxTexture, level, 0, face, &offset);
		// 		RAY_MEXPECTS(ret == KTX_SUCCESS,
		// 			"[Skybox] Failed to get texture image offset at face {1}, mip level {1}", face,
		// 			level);
		// 		VkBufferImageCopy bufferCopyRegion = {};
		// 		bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		// 		bufferCopyRegion.imageSubresource.mipLevel = level;
		// 		bufferCopyRegion.imageSubresource.baseArrayLayer = face;
		// 		bufferCopyRegion.imageSubresource.layerCount = 1;
		// 		bufferCopyRegion.imageExtent.width = ktxTexture->baseWidth >> level;
		// 		bufferCopyRegion.imageExtent.height = ktxTexture->baseHeight >> level;
		// 		bufferCopyRegion.imageExtent.depth = 1;
		// 		bufferCopyRegion.bufferOffset = offset;
		// 		bufferCopyRegions.emplace_back(bufferCopyRegion);
		// 	}
		// }

		// CopyFromBuffer(stagingBuffer, bufferCopyRegions);
		// TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// m_Sampler = MakeScope<TextureSampler>(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		// 	VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		// 	(float)GetMipLevels());

		// ktxTexture_Destroy(ktxTexture);
	}
}  // namespace At0::Ray
