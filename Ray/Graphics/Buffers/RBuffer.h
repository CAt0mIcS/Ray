#pragma once

#include "../../RBase.h"
#include "../Core/RRenderContext.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class CommandPool;

	class RAY_EXPORT Buffer : NonCopyable
	{
	public:
		Buffer(const RenderContext& context, VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties, const void* data = nullptr);
		Buffer(const RenderContext& context, VkDeviceSize size);
		Buffer(const RenderContext& context);
		virtual ~Buffer();

		void MapMemory(
			void** data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
		void UnmapMemory() const;
		void FlushMemory(VkDeviceSize size = VK_WHOLE_SIZE, uint32_t offset = 0) const;
		void Update(void* data, VkDeviceSize size, VkDeviceSize offset);
		void CopyRange(VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size);

		VkMemoryPropertyFlags GetMemoryProperties() const { return m_MemoryProperties; }

		static void MapMemory(const RenderContext& context, void** data, VkDeviceMemory memory,
			VkDeviceSize size, VkDeviceSize offset = 0);
		static void UnmapMemory(const RenderContext& context, VkDeviceMemory memory);
		static void FlushMemory(const RenderContext& context, VkDeviceMemory bufferMemory,
			VkDeviceSize size = VK_WHOLE_SIZE, uint32_t offset = 0);

		static void CreateBuffer(const RenderContext& context, VkDeviceSize size,
			VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
			VkDeviceMemory& bufferMemory);
		static void CopyBuffer(const RenderContext& context, CommandPool& transientCommandPool,
			VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		static uint32_t PadSizeToAlignment(uint32_t originalSize, uint32_t alignment);
		static void BindBufferToMemory(
			const RenderContext& context, VkBuffer buffer, VkDeviceMemory memory);

		const VkBuffer& GetBuffer() const { return m_Buffer; }
		VkDeviceSize GetSize() const { return m_Size; }
		const VkDeviceMemory& GetMemory() const { return m_BufferMemory; }
		void* GetMapped() { return m_Mapped; }
		const void* GetMapped() const { return m_Mapped; }
		operator const VkBuffer() const { return m_Buffer; }

		const RenderContext& GetRenderContext() const { return m_Context; }

	protected:
		void Destroy();
		VkMemoryPropertyFlags ValidateMemoryProperties() const;

	protected:
		static uint32_t s_NonCoherentAtomSize;

		const RenderContext& m_Context;

		VkBuffer m_Buffer = VK_NULL_HANDLE;
		VkDeviceMemory m_BufferMemory = VK_NULL_HANDLE;

		VkMemoryPropertyFlags m_MemoryProperties = 0;
		VkDeviceSize m_Size = 0;
		void* m_Mapped = nullptr;
	};
}  // namespace At0::Ray
