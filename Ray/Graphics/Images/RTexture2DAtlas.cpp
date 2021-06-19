#include "Rpch.h"
#include "RTexture2DAtlas.h"

#include "Graphics/Buffers/RBuffer.h"
#include "Utils/RException.h"


namespace At0::Ray
{
	Texture2DAtlas::Texture2DAtlas(UInt2 extent, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags memProps)
		: Texture2D(extent, format, tiling, usage, memProps)
	{
		m_FreeAreas.emplace_back(UInt2{ 0, 0 }, extent);
	}

	void Texture2DAtlas::Emplace(UInt2 extent, uint8_t* pixels)
	{
		Area* area = AllocateArea(extent);

		if (!area)
			ThrowRuntime("[Texture2DAtlas] Texture atlas too small.");

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
	}

	Texture2DAtlas::Area* Texture2DAtlas::AllocateArea(UInt2 extent)
	{
		auto getFreeArea = [this](UInt2 extent) {
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

		Area oldArea = getFreeArea(extent);

		// Unable to find free area
		if (oldArea.size.x == 0)
		{
			// collapseFreeAreas();
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
