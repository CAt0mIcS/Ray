#pragma once

#include "../../../RBase.h"
#include "../RShader.h"

#include "../../Buffers/RBufferSynchronizer.h"


namespace At0::Ray
{
	class Pipeline;
	class Buffer;

	class RAY_EXPORT BufferUniform
	{
	private:
		class ProxyType
		{
		public:
			ProxyType(BufferUniform* pThis, uint32_t offsetInUniformBlock)
				: m_BufferUniform(pThis), m_OffsetInUniformBlock(offsetInUniformBlock)
			{
			}

			template<typename T>
			ProxyType& operator=(T&& data)
			{
				BufferSynchronizer::Get().Update(
					data, m_BufferUniform->GetOffset() + m_OffsetInUniformBlock);
				return *this;
			}

		private:
			BufferUniform* m_BufferUniform;
			uint32_t m_OffsetInUniformBlock;
		};

	public:
		BufferUniform(std::string_view name, Shader::Stage stage, const Pipeline& pipeline);
		BufferUniform(std::string_view name, uint32_t binding, uint32_t size,
			std::unordered_map<std::string, uint32_t> uniformInBlockOffsets);

		/**
		 * @returns Name of the uniform block
		 */
		std::string_view GetName() const { return m_Name; }

		/**
		 * @returns Binding of the uniform block
		 */
		uint32_t GetBinding() const { return m_Binding; }

		/**
		 * This offset does not take into account multiple buffers allocated from the dynamic
		 * uniform buffer and is the offset from the first buffer
		 * @returns Global offset in the uniform buffer.
		 */
		uint32_t GetGlobalOffset() const { return m_Offset; }

		/**
		 * @returns Offset in the buffer received when calling BufferUniform::GetBuffer
		 */
		uint32_t GetOffset() const;

		/**
		 * @returns Buffer where the uniform data is stored
		 */
		const Buffer& GetBuffer() const;

		/**
		 * @returns Size in bytes of the uniform block
		 */
		uint32_t GetSize() const { return m_Size; }

		/**
		 * @param name Name of the uniform to update in the uniform block
		 * @returns Assignable type to update uniforms in the uniform block
		 */
		ProxyType operator[](std::string_view name);

	private:
		void Setup(uint32_t bufferSize);

	private:
		std::string m_Name;

		// The binding specified in the shader
		uint32_t m_Binding;
		// The offset in the global uniform buffer
		uint32_t m_Offset;
		// Size in bytes of the uniform block
		uint32_t m_Size;

		// Keeps track of all the uniforms in a uniform block and their offset in the block
		std::unordered_map<std::string, uint32_t> m_UniformInBlockOffsets;
	};
}  // namespace At0::Ray
