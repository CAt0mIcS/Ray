#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"
#include "../../Core/RMath.h"
#include "../../Resources/RImage2D.h"

#include <vulkan/vulkan_core.h>
#include <vector>


namespace At0::Ray
{
	class ImageDepth;
	class Renderpass;
	class RenderStage;
	class Swapchain;

	class RAY_EXPORT Framebuffers : NonCopyable
	{
	public:
		Framebuffers(const UInt2& extent, const RenderStage& renderStage,
			const Renderpass& renderPass, const Swapchain& swapchain,
			const ImageDepth& depthStencil, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
		~Framebuffers();

		Image2D* GetAttachment(uint32_t index) const { return m_ImageAttachments[index].get(); }

		const std::vector<Scope<Image2D>>& GetImageAttachments() const
		{
			return m_ImageAttachments;
		}
		const std::vector<VkFramebuffer>& GetFramebuffers() const { return m_Framebuffers; }

	private:
		std::vector<Scope<Image2D>> m_ImageAttachments;
		std::vector<VkFramebuffer> m_Framebuffers;
	};
}  // namespace At0::Ray
