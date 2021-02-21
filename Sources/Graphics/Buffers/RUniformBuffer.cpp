#include "Rpch.h"
#include "RUniformBuffer.h"


namespace At0::Ray
{
	UniformBuffer::UniformBuffer()
		: DynamicBuffer(s_InitialSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	{
	}
}  // namespace At0::Ray
