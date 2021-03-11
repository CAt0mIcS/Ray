#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"
#include "../Core/RBindable.h"

#include "RShader.h"

#include <unordered_map>
#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT Pipeline : public Bindable
	{
	public:
		enum class BindPoint
		{
			Graphics = 0,
			Compute = 1,
			RayTracing = 1000165000
		};

	public:
		virtual ~Pipeline();

		virtual VkDescriptorSetLayout GetDescriptorSetLayout(uint32_t set) const
		{
			return VK_NULL_HANDLE;
		}
		virtual VkDescriptorPool GetDescriptorPool() const { return VK_NULL_HANDLE; };
		virtual Pipeline::BindPoint GetBindPoint() const = 0;
		void CmdBind(const CommandBuffer& cmdBuff) const override;

		const Shader& GetShader() const { return m_Shader; }
		const VkPipelineLayout& GetLayout() const { return m_Layout; }

		operator const VkPipeline&() const { return m_Pipeline; }

	protected:
		Pipeline() = default;

	protected:
		VkPipeline m_Pipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_Layout = VK_NULL_HANDLE;

		Shader m_Shader;
	};
}  // namespace At0::Ray
