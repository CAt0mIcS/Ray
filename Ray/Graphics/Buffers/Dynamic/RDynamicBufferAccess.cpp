#include "Rpch.h"
#include "RDynamicBufferAccess.h"
#include "RDynamicBuffer.h"


namespace At0::Ray
{
	DynamicBufferAccess::DynamicBufferAccess(DynamicBuffer* pBuffer, uint32_t offsetInUniformBlock)
		: m_Buffer(pBuffer), m_OffsetInUniformBlock(offsetInUniformBlock)
	{
	}

	void DynamicBufferAccess::Update(void* data, uint32_t size)
	{
		m_Buffer->Update(data, size, m_OffsetInUniformBlock);
	}
}  // namespace At0::Ray