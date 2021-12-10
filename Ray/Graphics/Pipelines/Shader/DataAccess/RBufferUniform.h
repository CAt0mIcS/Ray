#pragma once

#include "../../../../RBase.h"
#include "../../../Buffers/Dynamic/RDynamicUniformBuffer.h"


namespace At0::Ray
{
	class Pipeline;
	class Buffer;
	class CommandBuffer;
	enum class ShaderStage;

	class RAY_EXPORT BufferUniform
	{
	public:
		class AccessType
		{
		public:
			AccessType(DynamicBuffer* pBuffer, uint32_t offsetInUniformBlock)
				: m_Buffer(pBuffer), m_OffsetInUniformBlock(offsetInUniformBlock)
			{
			}

			template<typename T>
			AccessType& operator=(T&& data)
			{
				m_Buffer->Update(data, m_OffsetInUniformBlock);
				return *this;
			}

		private:
			DynamicBuffer* m_Buffer;
			uint32_t m_OffsetInUniformBlock;
		};

	public:
		BufferUniform(std::string_view name, ShaderStage stage, const Pipeline& pipeline);
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
		 * @returns Global offset in the single global uniform buffer
		 */
		uint32_t GetOffset() const { return m_UniformBuffer->GetOffset(); }

		/**
		 * @returns Shader stage of this uniform
		 */
		ShaderStage GetShaderStage() const { return m_ShaderStage; }

		/**
		 * @returns Buffer where the uniform data is stored
		 */
		DynamicUniformBuffer& GetUniformBuffer() { return *m_UniformBuffer; }
		const DynamicUniformBuffer& GetUniformBuffer() const { return *m_UniformBuffer; }

		/**
		 * @returns Size in bytes of the uniform block
		 */
		uint32_t GetSize() const { return m_UniformBuffer->GetSize(); }

		/**
		 * @param name Name of the uniform to update in the uniform block
		 * @returns Assignable type to update uniforms in the uniform block
		 */
		AccessType operator[](const std::string& name);

		/**
		 * Writes data directly into the buffer
		 */
		template<typename T>
		void Update(T&& data, VkDeviceSize offset = 0) requires(!std::is_same_v<T, void*>)
		{
			Update((void*)&data, (uint32_t)sizeof(data), offset);
		}

		/**
		 * Writes raw data directly into the buffer
		 */
		void Update(void* data, VkDeviceSize size, VkDeviceSize offset = 0);

		/**
		 * @returns If the uniform block contains a variable with name
		 */
		bool Has(const std::string& name);

		/**
		 * If this buffer uniform represents a push constant the function will bind it to the
		 * command buffer
		 */
		void CmdBindAsPushConstant(
			const CommandBuffer& cmdBuff, VkPipelineLayout pipelineLayout) const;

	private:
		void Setup(uint32_t bufferSize);

	private:
		std::string m_Name;

		// The binding specified in the shader
		uint32_t m_Binding;

		// Memory range in global uniform buffer
		Scope<DynamicUniformBuffer> m_UniformBuffer;

		// Keeps track of all the uniforms in a uniform block and their offset in the block
		std::vector<std::pair<std::string, uint32_t>> m_UniformInBlockOffsets;

		ShaderStage m_ShaderStage;
	};
}  // namespace At0::Ray
