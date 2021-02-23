#include "Rpch.h"
#include "RUniformBufferSynchronizer.h"


namespace At0::Ray
{
	void UniformBufferSynchronizer::Create() { s_Instance = new UniformBufferSynchronizer(); }
	UniformBufferSynchronizer& UniformBufferSynchronizer::Get() { return *s_Instance; }

	void UniformBufferSynchronizer::Destroy() { delete s_Instance; }
}  // namespace At0::Ray
