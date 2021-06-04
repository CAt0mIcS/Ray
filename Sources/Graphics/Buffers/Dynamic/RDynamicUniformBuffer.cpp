#include "Rpch.h"
#include "RDynamicUniformBuffer.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RPhysicalDevice.h"

#include "../RBufferSynchronizer.h"


namespace At0::Ray
{
	Ref<UniformBuffer> DynamicUniformBuffer::s_UniformBuffer = nullptr;
	std::atomic<VkDeviceSize> DynamicUniformBuffer::s_NextOffset = 0;

	DynamicUniformBuffer::DynamicUniformBuffer(VkDeviceSize size)
		: DynamicBuffer(s_UniformBuffer, size, s_NextOffset)
	{
		if (!s_UniformBuffer)
			s_UniformBuffer = MakeRef<UniformBuffer>();

		s_NextOffset +=
			Buffer::PadSizeToAlignment(size, Graphics::Get()
												 .GetPhysicalDevice()
												 .GetProperties()
												 .limits.minUniformBufferOffsetAlignment);
	}
}  // namespace At0::Ray
