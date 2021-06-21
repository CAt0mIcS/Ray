#pragma once

#include "../../../RBase.h"
#include "../../../Utils/RAssert.h"

#include <vulkan/vulkan_core.h>


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
		void Update(T&& data, VkDeviceSize offset = 0)
		{
			Update((void*)&data, sizeof(T), offset);
		}

		/**
		 * Updates the buffer with data
		 * @param data The data to insert into the buffer
		 * @param offset Local offset
		 */
		void Update(void* data, VkDeviceSize size, VkDeviceSize offset = 0);

		const Buffer& GetBuffer() const { return *m_Buffer; }
		Buffer& GetBuffer() { return *m_Buffer; }
		VkDeviceSize GetSize() const { return m_Size; }
		VkDeviceSize GetOffset() const { return m_Offset; }

	protected:
		DynamicBuffer() = default;
		void Init(Ref<Buffer> buffer, VkDeviceSize size, VkDeviceSize offset);

	private:
		// Size in bytes of part of the dynamic buffer
		VkDeviceSize m_Size = 0;

		// Offset in the static buffer this class uses
		VkDeviceSize m_Offset = 0;

		// Static buffer, thread-safe
		Ref<Buffer> m_Buffer = nullptr;
	};
}  // namespace At0::Ray
