﻿#pragma once

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
		static UniformBufferSynchronizer& Get();

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

	private:
		UniformBufferSynchronizer() = default;
		~UniformBufferSynchronizer() = default;

	private:
		UniformBuffer m_UniformBuffer;
	};
}  // namespace At0::Ray