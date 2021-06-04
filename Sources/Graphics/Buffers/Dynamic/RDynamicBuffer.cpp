#include "Rpch.h"
#include "RDynamicBuffer.h"

#include "../RBuffer.h"


namespace At0::Ray
{
	void DynamicBuffer::Update(void* data, VkDeviceSize offset)
	{
		m_Buffer->Update(data, m_Size, m_Offset + offset);
	}

	DynamicBuffer::DynamicBuffer(Ref<Buffer> buffer, VkDeviceSize size, VkDeviceSize offset)
		: m_Buffer(buffer), m_Size(size), m_Offset(offset)
	{
	}
}  // namespace At0::Ray
