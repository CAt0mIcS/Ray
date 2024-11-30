#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"
#include "../Core/RSharedBindable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class Shader;
	class RenderContext;

	class RAY_EXPORT Pipeline : public SharedBindable, NonCopyable
	{
	public:
		enum class BindPoint
		{
			Graphics = VK_PIPELINE_BIND_POINT_GRAPHICS,
			Compute = VK_PIPELINE_BIND_POINT_COMPUTE,
			RayTracing = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR
		};

	public:
		virtual ~Pipeline();

		virtual VkDescriptorSetLayout GetDescriptorSetLayout(uint32_t set) const;
		virtual VkDescriptorPool GetDescriptorPool() const { return VK_NULL_HANDLE; };
		virtual Pipeline::BindPoint GetBindPoint() const = 0;
		void CmdBind(const CommandBuffer& cmdBuff) const override;

		const Shader& GetShader() const { return *m_Shader; }
		const VkPipelineLayout& GetLayout() const { return m_Layout; }

		operator const VkPipeline&() const { return m_Pipeline; }

	protected:
		Pipeline(Ref<Shader> shader);

	protected:
		VkPipeline m_Pipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_Layout = VK_NULL_HANDLE;

		Ref<Shader> m_Shader;
	};
}  // namespace At0::Ray
