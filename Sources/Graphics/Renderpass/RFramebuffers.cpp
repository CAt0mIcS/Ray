#include "Rpch.h"
#include "RFramebuffers.h"

#include "Graphics/RRenderStage.h"
#include "Graphics/RGraphics.h"
#include "RSwapchain.h"
#include "RRenderpass.h"
#include "Resources/RImageDepth.h"

#include "Devices/Vulkan/RLogicalDevice.h"
#include "Debug/RException.h"


namespace At0::Ray
{
	Framebuffers::Framebuffers(const UInt2& extent, const RenderStage& renderStage,
		const Renderpass& renderPass, const Swapchain& swapchain, const ImageDepth& depthStencil,
		VkSampleCountFlagBits samples)
	{
		const LogicalDevice& logicalDevice = Graphics::Get().GetLogicalDevice();

		for (const auto& attachment : renderStage.GetAttachments())
		{
			auto attachmentSamples = attachment.IsMultisampled() ? samples : VK_SAMPLE_COUNT_1_BIT;

			switch (attachment.GetType())
			{
			case Attachment::Type::Image:
				m_ImageAttachments.emplace_back(MakeScope<Image2D>(extent, attachment.GetFormat(),
					VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
					VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
					VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, attachmentSamples));
				break;
			case Attachment::Type::Depth: m_ImageAttachments.emplace_back(nullptr); break;
			case Attachment::Type::Swapchain: m_ImageAttachments.emplace_back(nullptr); break;
			}
		}

		m_Framebuffers.resize(swapchain.GetImageCount());

		for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
		{
			std::vector<VkImageView> attachments;

			for (const auto& attachment : renderStage.GetAttachments())
			{
				switch (attachment.GetType())
				{
				case Attachment::Type::Image:
					attachments.emplace_back(GetAttachment(attachment.GetBinding())->GetView());
					break;
				case Attachment::Type::Depth:
					attachments.emplace_back(depthStencil.GetView());
					break;
				case Attachment::Type::Swapchain:
					attachments.emplace_back(swapchain.GetImageViews().at(i));
					break;
				}
			}

			VkFramebufferCreateInfo framebufferCreateInfo = {};
			framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferCreateInfo.renderPass = renderPass;
			framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferCreateInfo.pAttachments = attachments.data();
			framebufferCreateInfo.width = extent.x;
			framebufferCreateInfo.height = extent.y;
			framebufferCreateInfo.layers = 1;
			RAY_VK_THROW_FAILED(vkCreateFramebuffer(logicalDevice, &framebufferCreateInfo, nullptr,
									&m_Framebuffers[i]),
				"[Framebuffers] Failed to create.");
		}
	}

	Framebuffers::~Framebuffers() {}
}  // namespace At0::Ray
