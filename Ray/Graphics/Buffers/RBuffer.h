#pragma once

#include "Ray/RBase.h"
#include "RayBase/RNonCopyable.h"

#include <RayRenderer/Core/RBuffer.h>
#include <RayRenderer/Core/RDeviceMemory.h>


namespace At0::Ray
{
	class RAY_EXPORT Buffer : NonCopyable
	{
	public:
		Buffer(RrDeviceSize size, RrBufferUsageFlags usage, RrMemoryPropertyFlags properties,
			const void* data = nullptr);
		Buffer(RrDeviceSize size);
		Buffer() = default;
		virtual ~Buffer();

		void MapMemory(
			void** data, RrDeviceSize size = RR_WHOLE_SIZE, RrDeviceSize offset = 0) const;
		void UnmapMemory() const;
		void FlushMemory(RrDeviceSize size = RR_WHOLE_SIZE, RrDeviceSize offset = 0) const;
		void Update(void* data, RrDeviceSize size, RrDeviceSize offset);
		void CopyRange(RrDeviceSize srcOffset, RrDeviceSize dstOffset, RrDeviceSize size);

		RrMemoryPropertyFlags GetMemoryProperties() const { return m_MemoryProperties; }

		static void MapMemory(
			void** data, RrDeviceMemory memory, RrDeviceSize size, RrDeviceSize offset = 0);
		static void UnmapMemory(RrDeviceMemory memory);
		static void FlushMemory(RrDeviceMemory bufferMemory, RrDeviceSize size = RR_WHOLE_SIZE,
			RrDeviceSize offset = 0);

		static void CreateBuffer(RrDeviceSize size, RrBufferUsageFlags usage,
			RrMemoryPropertyFlags properties, RrBuffer& buffer, RrDeviceMemory& bufferMemory);
		static void CopyBuffer(RrBuffer srcBuffer, RrBuffer dstBuffer, RrDeviceSize size);
		static uint32_t PadSizeToAlignment(RrDeviceSize originalSize, RrDeviceSize alignment);
		static void BindBufferToMemory(RrBuffer buffer, RrDeviceMemory memory);

		const RrBuffer& GetBuffer() const { return m_Buffer; }
		RrDeviceSize GetSize() const { return m_Size; }
		const RrDeviceMemory& GetMemory() const { return m_BufferMemory; }
		void* GetMapped() { return m_Mapped; }
		const void* GetMapped() const { return m_Mapped; }
		operator const RrBuffer&() const { return m_Buffer; }

	protected:
		void Destroy();
		RrMemoryPropertyFlags ValidateMemoryProperties() const;

	protected:
		static uint32_t s_NonCoherentAtomSize;

		RrBuffer m_Buffer = nullptr;
		RrDeviceMemory m_BufferMemory = nullptr;

		RrMemoryPropertyFlags m_MemoryProperties = 0;
		RrDeviceSize m_Size = 0;
		void* m_Mapped = nullptr;
	};
}  // namespace At0::Ray
