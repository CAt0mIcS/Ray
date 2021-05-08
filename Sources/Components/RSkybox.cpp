#include "Rpch.h"
#include "RSkybox.h"

#include "Graphics/Images/RTexture2D.h"
#include "Graphics/Images/RImage.h"
#include "Graphics/Buffers/RBuffer.h"
#include "RMesh.h"

#include <ktx.h>
#include <ktxvulkan.h>


namespace At0::Ray
{
	Skybox::Skybox(Entity entity) : Component(entity)
	{
		std::string filepath = "Resources/Textures/cubemap_space.ktx";

		ktxResult result;
		ktxTexture* ktxTexture;

		result = ktxTexture_CreateFromNamedFile(
			filepath.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktxTexture);
		RAY_MEXPECTS(result == KTX_SUCCESS, "[Skybox] Failed to load texture \"{0}\"", filepath);

		ktx_uint8_t* ktxTextureData = ktxTexture_GetData(ktxTexture);
		ktx_size_t ktxTextureSize = ktxTexture_GetSize(ktxTexture);

		Buffer stagingBuffer((VkDeviceSize)ktxTextureSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			ktxTextureData);

		m_Texture = MakeRef<Image2D>(UInt2{ ktxTexture->baseWidth, ktxTexture->baseHeight },
			VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ktxTexture->numLevels, VK_IMAGE_ASPECT_COLOR_BIT,
			6,	// Array layers count as cube faces in vulkan
			VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT);

		// Setup buffer copy regions for each face including all of its miplevels
		std::vector<VkBufferImageCopy> bufferCopyRegions;
		uint32_t offset = 0;

		for (uint32_t face = 0; face < 6; face++)
		{
			for (uint32_t level = 0; level < m_Texture->GetMipLevels(); level++)
			{
				// Calculate offset into staging buffer for the current mip level and face
				ktx_size_t offset;
				KTX_error_code ret = ktxTexture_GetImageOffset(ktxTexture, level, 0, face, &offset);
				assert(ret == KTX_SUCCESS);
				VkBufferImageCopy bufferCopyRegion = {};
				bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				bufferCopyRegion.imageSubresource.mipLevel = level;
				bufferCopyRegion.imageSubresource.baseArrayLayer = face;
				bufferCopyRegion.imageSubresource.layerCount = 1;
				bufferCopyRegion.imageExtent.width = ktxTexture->baseWidth >> level;
				bufferCopyRegion.imageExtent.height = ktxTexture->baseHeight >> level;
				bufferCopyRegion.imageExtent.depth = 1;
				bufferCopyRegion.bufferOffset = offset;
				bufferCopyRegions.push_back(bufferCopyRegion);
			}
		}

		m_Texture->CopyFromBuffer(stagingBuffer, bufferCopyRegions);
		m_Texture->TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		ktxTexture_Destroy(ktxTexture);


		GetEntity().Emplace<Mesh>(Mesh::Cube());
	}
}  // namespace At0::Ray
