#pragma once

#include "RUniform.h"


namespace At0::Ray
{
	class RAY_EXPORT BufferUniform : public Uniform
	{
		friend class ProxyType;

	private:
		class ProxyType
		{
		public:
			ProxyType(BufferUniform* pUpper, std::string_view uniformName)
				: m_BufferUniform(pUpper), m_UniformName(uniformName)
			{
			}

			template<typename T>
			ProxyType& operator=(T&& data)
			{
				BufferSynchronizer::Get().Update(
					data, m_BufferUniform->GetGlobalBufferOffset() +
							  m_BufferUniform->GetUniformOffsetInBlock(m_UniformName));
				return *this;
			}

		private:
			BufferUniform* m_BufferUniform;
			std::string m_UniformName;
		};

	public:
		BufferUniform(std::string_view name, VkDescriptorSetLayout descSetLayout,
			VkDescriptorPool descSetPool, Pipeline::BindPoint bindPoint,
			VkPipelineLayout pipelineLayout, uint32_t bufferSize, uint32_t set, uint32_t binding);
		BufferUniform(std::string_view name, const Pipeline& pipeline, uint32_t bufferSize,
			uint32_t set, uint32_t binding);
		BufferUniform(
			std::string_view uniformBlockName, Shader::Stage stage, const Pipeline& pipeline);
		~BufferUniform();

		/**
		 * Updates the entire uniform block with data
		 */
		template<typename T>
		void Update(T&& data)
		{
			BufferSynchronizer::Get().Update(data, m_GlobalBufferOffset);
		}

		/**
		 * Updates the uniform with the name.
		 */
		template<typename T>
		void Update(std::string_view uniformName, T&& data)
		{
			BufferSynchronizer::Get().Update(
				data, m_GlobalBufferOffset + GetUniformOffsetInBlock(uniformName));
		}

		ProxyType operator[](std::string_view uniformName) { return { this, uniformName }; }

		BufferUniform& operator=(BufferUniform&& other) noexcept = default;
		BufferUniform(BufferUniform&& other) noexcept = default;

		BufferUniform& operator=(const BufferUniform& other) = default;
		BufferUniform(const BufferUniform& other) = default;

		uint32_t GetGlobalBufferOffset() const { return m_GlobalBufferOffset; }

	private:
		void Setup(uint32_t bufferSize, uint32_t binding);
		uint32_t GetUniformOffsetInBlock(std::string_view uniformName) const;

	private:
		uint32_t m_GlobalBufferOffset = (uint32_t)-1;

		/**
		 * A BufferUniform is always in a block in Vulkan.
		 * All uniforms in the uniform block are stored here too.
		 */
		std::optional<Shader::Uniforms> m_Uniforms = std::nullopt;
	};
}  // namespace At0::Ray
