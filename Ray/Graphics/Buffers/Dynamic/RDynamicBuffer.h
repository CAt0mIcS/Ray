#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RAssert.h"

#include <vulkan/vulkan_core.h>
#include <RayRenderer/Core/RCore.h>


namespace At0::Ray
{
	class Buffer;

	class RAY_EXPORT DynamicBuffer
	{
	public:
		virtual ~DynamicBuffer() = default;

		/**
		 * Updates the buffer with data
		 * @param data The data to insert into the buffer
		 * @param offset Local offset
		 */
		template<typename T>
		void Update(T&& data, RrDeviceSize offset = 0) requires(!std::is_same_v<T, void*>)
		{
			Update((void*)&data, sizeof(T), offset);
		}

		/**
		 * Updates the buffer with data
		 * @param data The data to insert into the buffer
		 * @param offset Local offset
		 */
		void Update(void* data, RrDeviceSize size, RrDeviceSize offset);

		const Buffer& GetBuffer() const { return *m_Buffer; }
		Buffer& GetBuffer() { return *m_Buffer; }
		RrDeviceSize GetSize() const { return m_Size; }
		RrDeviceSize GetOffset() const { return m_Offset; }

	protected:
		DynamicBuffer() = default;
		void Init(Ref<Buffer> buffer, RrDeviceSize size, RrDeviceSize offset);

	private:
		// Size in bytes of part of the dynamic buffer
		RrDeviceSize m_Size = 0;

		// Offset in the static buffer this class uses
		RrDeviceSize m_Offset = 0;

		// Static buffer, thread-safe
		Ref<Buffer> m_Buffer = nullptr;
	};
}  // namespace At0::Ray
