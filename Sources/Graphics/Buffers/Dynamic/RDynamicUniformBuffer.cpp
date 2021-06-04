#include "Rpch.h"
#include "RDynamicUniformBuffer.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RPhysicalDevice.h"


namespace At0::Ray
{
	Ref<UniformBuffer> DynamicUniformBuffer::s_UniformBuffer = nullptr;
	std::atomic<VkDeviceSize> DynamicUniformBuffer::s_NextOffset = 0;

	DynamicUniformBuffer::DynamicUniformBuffer(VkDeviceSize size)
		: DynamicBuffer(size, s_NextOffset)
	{
		if (!s_UniformBuffer)
			s_UniformBuffer = MakeRef<UniformBuffer>();
		m_Buffer = s_UniformBuffer;

		s_NextOffset +=
			Buffer::PadSizeToAlignment(size, Graphics::Get()
												 .GetPhysicalDevice()
												 .GetProperties()
												 .limits.minUniformBufferOffsetAlignment);
	}

	void DynamicUniformBuffer::Reset()
	{
		s_UniformBuffer.reset();
		s_NextOffset = 0;
	}
}  // namespace At0::Ray
