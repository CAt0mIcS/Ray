#include "Rpch.h"
#include "RBufferSynchronizer.h"


namespace At0::Ray
{
	void BufferSynchronizer::Create() { s_Instance = new BufferSynchronizer(); }
	BufferSynchronizer& BufferSynchronizer::Get() { return *s_Instance; }

	void BufferSynchronizer::Destroy() { delete s_Instance; }

	BufferSynchronizer::BufferSynchronizer() {}
}  // namespace At0::Ray
