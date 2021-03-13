#pragma once

#include "../../RBase.h"

#include "../Core/RBindable.h"
#include "RPipeline.h"
#include "RDescriptor.h"

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
		BufferUniform(std::string_view uniformName, Shader::Stage stage, const Pipeline& pipeline);
		~BufferUniform();

		BufferUniform& operator=(BufferUniform&& other) noexcept = default;
		BufferUniform(BufferUniform&& other) noexcept = default;

		uint32_t GetGlobalBufferOffset() const { return m_GlobalBufferOffset; }

	private:
		void Setup(uint32_t bufferSize, uint32_t binding);

	private:
		uint32_t m_GlobalBufferOffset;
	};


	class RAY_EXPORT ImageSamplerUniform : public Uniform
	{
	public:
		~ImageSamplerUniform();

	private:
	};

}  // namespace At0::Ray
