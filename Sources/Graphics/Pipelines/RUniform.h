#pragma once

#include "../../RBase.h"

#include "../Core/RBindable.h"
#include "RPipeline.h"
#include "RDescriptor.h"
#include "../Buffers/RBufferSynchronizer.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT Uniform : public Bindable
	{
	public:
		virtual ~Uniform();
		void CmdBind(const CommandBuffer& cmdBuff) const override;

		Uniform& operator=(Uniform&& other) noexcept;
		Uniform(Uniform&& other) noexcept;

	protected:
		Uniform() = default;

	protected:
		Scope<DescriptorSet> m_DescriptorSet;
	};


	class RAY_EXPORT BufferUniform : public Uniform
	{
	public:
		BufferUniform(VkDescriptorSetLayout descSetLayout, VkDescriptorPool descSetPool,
			Pipeline::BindPoint bindPoint, VkPipelineLayout pipelineLayout, uint32_t bufferSize,
			uint32_t set, uint32_t binding);
		BufferUniform(
			const Pipeline& pipeline, uint32_t bufferSize, uint32_t set, uint32_t binding);
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

		BufferUniform& operator=(BufferUniform&& other) noexcept = default;
		BufferUniform(BufferUniform&& other) noexcept = default;

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


	class RAY_EXPORT ImageSamplerUniform : public Uniform
	{
	public:
		~ImageSamplerUniform();

	private:
	};

}  // namespace At0::Ray
