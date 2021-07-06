#include "Rpch.h"
#include "RDynamicUniformBuffer.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RPhysicalDevice.h"


namespace At0::Ray
{
	Ref<UniformBuffer> DynamicUniformBuffer::s_UniformBuffer = nullptr;
	std::atomic<RrDeviceSize> DynamicUniformBuffer::s_NextOffset = 0;

	DynamicUniformBuffer::DynamicUniformBuffer(RrDeviceSize size)
	{
		if (!s_UniformBuffer)
			s_UniformBuffer = MakeRef<UniformBuffer>();

		RrDeviceSize offset = s_NextOffset;
		s_NextOffset +=
			Buffer::PadSizeToAlignment(size, Graphics::Get()
												 .GetPhysicalDevice()
												 .GetProperties()
												 .limits.minUniformBufferOffsetAlignment);

		DynamicBuffer::Init(s_UniformBuffer, size, offset);
	}

	void DynamicUniformBuffer::Reset()
	{
		s_UniformBuffer.reset();
		s_NextOffset = 0;
	}
}  // namespace At0::Ray
