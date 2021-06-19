#include "Rpch.h"
#include "RDynamicBuffer.h"

#include "../RBuffer.h"


namespace At0::Ray
{
	void DynamicBuffer::Update(void* data, VkDeviceSize size, VkDeviceSize offset)
	{
		RAY_MEXPECTS(
			size + offset <= m_Size, "[DynamicBuffer] Trying to update region outside of buffer.");

		m_Buffer->Update(data, size, m_Offset + offset);
	}

	void DynamicBuffer::Init(Ref<Buffer> buffer, VkDeviceSize size, VkDeviceSize offset)
	{
		m_Buffer = std::move(buffer);
		m_Size = size;
		m_Offset = offset;
	}
}  // namespace At0::Ray
