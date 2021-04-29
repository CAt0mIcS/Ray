#include "Rpch.h"
#include "RBufferSynchronizer.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"


namespace At0::Ray
{
	BufferSynchronizer* BufferSynchronizer::s_Instance = nullptr;

	void BufferSynchronizer::Create() { s_Instance = new BufferSynchronizer(); }
	BufferSynchronizer& BufferSynchronizer::Get() { return *s_Instance; }

	void BufferSynchronizer::Destroy() { delete s_Instance; }

	void BufferSynchronizer::Update(void* data, uint32_t size, uint32_t offset)
	{
		m_UniformBuffer.Update(data, size, offset);
	}

	void BufferSynchronizer::Emplace(uint32_t size, uint32_t alignment, uint32_t* offset)
	{
		m_UniformBuffer.Emplace(size, alignment, offset);
	}

	void BufferSynchronizer::Copy(uint32_t srcOffset, uint32_t dstOffset, uint32_t size)
	{
		void* uData = nullptr;
		m_UniformBuffer.MapMemory(&uData);

		memcpy((char*)uData + dstOffset, (char*)uData + srcOffset, size);
		m_UniformBuffer.UnmapMemory();
	}

	BufferSynchronizer::BufferSynchronizer() {}
}  // namespace At0::Ray
