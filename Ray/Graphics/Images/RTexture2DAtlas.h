#pragma once

#include "RTexture2D.h"

#include <deque>
#include <array>


namespace At0::Ray
{
	class RAY_EXPORT Texture2DAtlas : public Texture2D
	{
	public:
		struct Area
		{
			UInt2 pos;
			UInt2 size;
		};

	public:
		Texture2DAtlas(UInt2 extent, VkFormat format,
			VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
			VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VkMemoryPropertyFlags memProps = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		Area* Emplace(std::string_view filepath);
		Area* Emplace(UInt2 extent, uint8_t* pixels);

		std::array<Float2, 4> MapUV(const Area& area) const;

	private:
		Area* AllocateArea(UInt2 extent);

	private:
		std::deque<Area> m_FreeAreas;
		std::vector<Area> m_AllocatedAreas;
	};
}  // namespace At0::Ray
