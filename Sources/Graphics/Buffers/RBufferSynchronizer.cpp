#include "Rpch.h"
#include "RBufferSynchronizer.h"


namespace At0::Ray
{
	void BufferSynchronizer::Create() { s_Instance = new BufferSynchronizer(); }
	BufferSynchronizer& BufferSynchronizer::Get() { return *s_Instance; }

	void BufferSynchronizer::Destroy() { delete s_Instance; }

	void BufferSynchronizer::Emplace(
		uint32_t size, uint32_t alignment, uint32_t* offset, uint32_t* bufferID)
	{
		m_UniformBuffer.Emplace(size, alignment, offset, bufferID);
	}

	BufferSynchronizer::BufferSynchronizer() {}
}  // namespace At0::Ray
