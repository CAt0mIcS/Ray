#pragma once

#include "../../../RBase.h"


namespace At0::Ray
{
	class DynamicBuffer;

	class RAY_EXPORT DynamicBufferAccess
	{
	public:
		DynamicBufferAccess(DynamicBuffer* pBuffer, uint32_t offsetInUniformBlock);

		template<typename T>
		DynamicBufferAccess& operator=(T&& data)
		{
			Update((void*)&data, sizeof(data));
			return *this;
		}

	private:
		void Update(void* data, uint32_t size);

	private:
		DynamicBuffer* m_Buffer;
		uint32_t m_OffsetInUniformBlock;
	};
}

