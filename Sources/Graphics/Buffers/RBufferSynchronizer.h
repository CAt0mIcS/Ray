#pragma once

#include "../../RBase.h"
#include "RUniformBuffer.h"
#include "RIndexBuffer.h"
#include "RVertexBuffer.h"


namespace At0::Ray
{
	/**
	 * Handles the global buffers to ensure thread-safety
	 * RAY_TODO: Make thread-safe
	 */
	class RAY_EXPORT BufferSynchronizer
	{
	public:
		static void Create();
		static BufferSynchronizer& Get();
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

		/**
		 * Allocates allocSize of bytes at the end of the buffer
		 * Resizes the buffer if the capacity is not enough
		 * @param allocSize The size to free
		 * @param alignment The minimum alignment requirements in bytes
		 * @param offset Writes to offset the offset in the global buffer
		 */
		void Emplace(uint32_t size, uint32_t alignment, uint32_t* offset);

		// const VertexBuffer& GetVertexBuffer() const { return m_VertexBuffer; }
		// const IndexBuffer& GetIndexBuffer() const { return m_IndexBuffer; }
		const UniformBuffer& GetUniformBuffer() const { return m_UniformBuffer; }

	private:
		BufferSynchronizer();
		~BufferSynchronizer() = default;

	private:
		UniformBuffer m_UniformBuffer;
		inline static BufferSynchronizer* s_Instance;
	};
}  // namespace At0::Ray
