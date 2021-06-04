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
			RAY_MEXPECTS(sizeof(data) == m_Size,
				"[DynamicBuffer] Trying to update buffer region of {0} byte(s) with data that has "
				"{1} byte(s)",
				m_Size, sizeof(data));
			Update((void*)&data, offset);
		}

		/**
		 * Updates the buffer with data
		 * @param data The data to insert into the buffer
		 * @param offset Local offset
		 */
		void Update(void* data, VkDeviceSize offset = 0);

		const Buffer& GetBuffer() const { return *m_Buffer; }
		VkDeviceSize GetSize() const { return m_Size; }
		VkDeviceSize GetOffset() const { return m_Offset; }

	protected:
		DynamicBuffer(Ref<Buffer> buffer, VkDeviceSize size, VkDeviceSize offset);

	private:
		Ref<Buffer> m_Buffer = nullptr;
		VkDeviceSize m_Size = 0;
		VkDeviceSize m_Offset = 0;
	};
}  // namespace At0::Ray
