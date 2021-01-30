#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <optional>
#include <vector>

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class ImageDepth;
	class RenderStage;

	class RAY_EXPORT Renderpass
	{
	public:
		class SubpassDescription : NonCopyable
		{
		public:
			SubpassDescription(VkPipelineBindPoint bindPoint,
				std::vector<VkAttachmentReference> colorAttachments,
				const std::optional<uint32_t>& depthAttachment);

			const VkSubpassDescription& GetSubpassDescription() const
			{
				return m_SubpassDescription;
			}

		private:
			VkSubpassDescription m_SubpassDescription;
			std::vector<VkAttachmentReference> m_ColorAttachments;
			VkAttachmentReference m_DepthStencilAttachment{};
		};

	public:
		Renderpass(const RenderStage& renderStage, VkFormat depthFormat, VkFormat surfaceFormat,
			VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
		~Renderpass();

		operator const VkRenderPass&() const { return m_Renderpass; }
		const VkRenderPass& GetRenderpass() const { return m_Renderpass; }

	private:
		VkRenderPass m_Renderpass = VK_NULL_HANDLE;
	};
}  // namespace At0::Ray
