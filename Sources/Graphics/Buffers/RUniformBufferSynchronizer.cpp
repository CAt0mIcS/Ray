#include "Rpch.h"
#include "RUniformBufferSynchronizer.h"


namespace At0::Ray
{
	UniformBufferSynchronizer& UniformBufferSynchronizer::Get()
	{
		if (!s_Instance)
			s_Instance = new UniformBufferSynchronizer();
		return *s_Instance;
	}

	void UniformBufferSynchronizer::Destroy() { delete s_Instance; }
}  // namespace At0::Ray
