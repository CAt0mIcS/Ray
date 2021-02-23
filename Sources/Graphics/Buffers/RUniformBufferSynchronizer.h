#pragma once

#include "../../RBase.h"
#include "RUniformBuffer.h"


namespace At0::Ray
{
	/**
	 * Handles the global uniform buffer to ensure thread-safety
	 * RAY_TODO: Make thread-safe
	 */
	class RAY_EXPORT UniformBufferSynchronizer
	{
	public:
		static void Create();
		static UniformBufferSynchronizer& Get();
		static void Destroy();

		/**
		 * Updates the uniform buffer.
		 * @param data The data to insert into the uniform buffer
		 * @param offset The global offset of the data in the buffer
		 */
		template<typename T>
		void Update(T&& data, uint32_t offset)
		{
			m_UniformBuffer.Update(&data, sizeof(data), offset);
		}

		const UniformBuffer& GetBuffer() const { return m_UniformBuffer; }

	private:
		UniformBufferSynchronizer() = default;
		~UniformBufferSynchronizer() = default;

	private:
		UniformBuffer m_UniformBuffer;
		inline static UniformBufferSynchronizer* s_Instance;
	};
}  // namespace At0::Ray
