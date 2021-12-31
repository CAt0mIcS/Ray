#include "RTexture2DAtlas.h"

#include "Graphics/Buffers/RBuffer.h"


namespace At0::Ray
{
	Texture2DAtlas::Texture2DAtlas(UInt2 extent, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags memProps)
		: Texture(extent, VK_IMAGE_TYPE_2D, format, tiling, usage, memProps)
	{
		m_FreeAreas.emplace_back(UInt2{ 0, 0 }, extent);
	}

	Texture2DAtlas::Area* Texture2DAtlas::Emplace(UInt2 extent, uint8_t* pixels)
	{
		Area* area = AllocateArea(extent);

		if (!area)
		{
			Log::Error(
				"[Texture2DAtlas] Too small to hold (x={0} | y={1}) image", extent.x, extent.y);
			return nullptr;
		}

		VkDeviceSize imageSize = extent.x * extent.y * 4;

		Buffer stagingBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pixels);

		VkBufferImageCopy region{};

		region.imageSubresource.aspectMask = m_ImageAspect;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;	 // RAY_TODO: MipLevel
		region.imageOffset = { (int)area->pos.x, (int)area->pos.y, 0 };
		region.imageExtent = { extent.x, extent.y, 1 };

		CopyFromBuffer(stagingBuffer, { region });
		TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		return &m_AllocatedAreas.front();
	}

	std::array<Float2, 4> Texture2DAtlas::MapUV(const Area& area) const
	{
		std::array<Float2, 4> uvs;

		Float2 areaPos = (Float2)area.pos;
		Float2 areaSize = (Float2)area.size;

		float bottomRightX = (areaPos.x + areaSize.x) / m_Extent.x;
		float bottomRightY = (areaPos.y + areaSize.y) / m_Extent.y;

		float bottomLeftX = areaPos.x / m_Extent.x;
		float bottomLeftY = (areaPos.y + areaSize.y) / m_Extent.y;

		float topLeftX = areaPos.x / m_Extent.x;
		float topLeftY = areaPos.y / m_Extent.y;

		float topRightX = (areaPos.x + areaSize.x) / m_Extent.x;
		float topRightY = areaPos.y / m_Extent.y;

		uvs[0] = { bottomRightX, bottomRightY };
		uvs[1] = { bottomLeftX, bottomLeftY };
		uvs[2] = { topLeftX, topLeftY };
		uvs[3] = { topRightX, topRightY };
		return uvs;
	}

	Texture2DAtlas::Area* Texture2DAtlas::AllocateArea(UInt2 extent)
	{
		auto getFreeArea = [this](UInt2 extent)
		{
			// Check if we have any free areas
			for (auto it = m_FreeAreas.begin(); it != m_FreeAreas.end(); ++it)
			{
				if ((*it).size.x >= extent.x && (*it).size.y >= extent.y)
				{
					Area oldArea = *it;
					m_FreeAreas.erase(it);
					return oldArea;
				}
			}
			return Area{};
		};

		auto isAdjacent = [this](const Area& first, const Area& other)
		{
			uint32_t x = first.pos.x;
			uint32_t y = first.pos.y;
			uint32_t width = first.size.x;
			uint32_t height = first.size.y;
			if (x == other.pos.x && width == other.size.x && y + height == other.pos.y)
				// Other is immediately below
				return true;
			if (x == other.pos.x && width == other.size.x && other.pos.y + other.size.y == y)
				// Other is immediately above
				return true;
			if (y == other.pos.y && height == other.size.y && x + width == other.pos.x)
				// Other is immediately to the right
				return true;
			if (y == other.pos.y && height == other.size.y && other.pos.x + other.size.x == x)
				// Other is immediately to the left
				return true;
			return false;
		};

		auto findAdjacent = [this, &isAdjacent](const Area& area)
		{
			for (auto it = m_FreeAreas.cbegin(); it != m_FreeAreas.cend(); ++it)
			{
				if (isAdjacent(area, *it))
					return it;
			}
			return m_FreeAreas.cend();
		};

		auto combine = [this](Area& first, const Area& other)
		{
			uint32_t& x = first.pos.x;
			uint32_t& y = first.pos.y;
			uint32_t& width = first.size.x;
			uint32_t& height = first.size.y;
			if (x == other.pos.x && width == other.size.x && y + height == other.pos.y)
			{
				// Other is immediately below
				height += other.size.y;
				return true;
			}
			if (x == other.pos.x && width == other.size.x && other.pos.y + other.size.y == y)
			{
				// Other is immediately above
				y -= other.size.y;
				height += other.size.y;
				return true;
			}
			if (y == other.pos.y && height == other.size.y && x + width == other.pos.x)
			{
				// Other is immediately to the right
				width += other.size.x;
				return true;
			}
			if (y == other.pos.y && height == other.size.y && other.pos.x + other.size.x == x)
			{
				// Other is immediately to the left
				x -= other.size.x;
				width += other.size.x;
				return true;
			}
			return false;
		};

		auto collapseFreeAreas = [this, &findAdjacent, &combine]()
		{
			if (m_FreeAreas.size() < 2)
				return;

			int collapsed = 0;
			do
			{
				collapsed = 0;
				std::deque<Area> collapsedAreas;
				while (!m_FreeAreas.empty())
				{
					Area first = m_FreeAreas.front();
					m_FreeAreas.pop_front();
					while (!m_FreeAreas.empty())
					{
						auto other = findAdjacent(first);
						if (other != m_FreeAreas.end())
						{
							combine(first, *other);
							m_FreeAreas.erase(other);
							++collapsed;
						}
						else
							break;
					}
					collapsedAreas.emplace_back(first);
				}
				m_FreeAreas = collapsedAreas;
			} while (collapsed > 0);
		};

		Area oldArea = getFreeArea(extent);

		// Unable to find free area
		if (oldArea.size.x == 0)
		{
			collapseFreeAreas();
			oldArea = getFreeArea(extent);
		}

		// Still unable to find free area
		if (oldArea.size.x == 0)
			return nullptr;

		m_AllocatedAreas.insert(m_AllocatedAreas.begin(), { oldArea.pos, extent });

		if (oldArea.size.x > extent.x)
		{
			// Add an area to the right of newly allocated area
			m_FreeAreas.emplace_back(UInt2{ oldArea.pos.x + extent.x, oldArea.pos.y },
				UInt2{ oldArea.size.x - extent.x, extent.y });
		}
		if (oldArea.size.y > extent.y)
		{
			// Add an area below the newly allocated area
			m_FreeAreas.emplace_back(UInt2{ oldArea.pos.x, oldArea.pos.y + extent.y },
				UInt2{ extent.x, oldArea.size.y - extent.y });
		}
		if (oldArea.size.x > extent.x && oldArea.size.y > extent.y)
		{
			// Add an area diagonally to the right and below the newly allocated area
			m_FreeAreas.emplace_back(UInt2{ oldArea.pos.x + extent.x, oldArea.pos.y + extent.y },
				UInt2{ oldArea.size.x - extent.x, oldArea.size.y - extent.y });
		}
		return &m_AllocatedAreas.front();
	}
}  // namespace At0::Ray
