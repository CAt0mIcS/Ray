#include "Rpch.h"
#include "RUniformBufferSynchronizer.h"


namespace At0::Ray
{
	UniformBufferSynchronizer& UniformBufferSynchronizer::Get()
	{
		static UniformBufferSynchronizer instance;
		return instance;
	}
}  // namespace At0::Ray
