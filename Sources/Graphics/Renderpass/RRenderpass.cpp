#include "Rpch.h"
#include "RRenderpass.h"

#include "Graphics/RRenderStage.h"
#include "Debug/RLogger.h"
#include "Debug/RException.h"
#include "Graphics/RGraphics.h"
#include "Devices/Vulkan/RLogicalDevice.h"

#include <vulkan/vulkan.h>


namespace At0::Ray
{
	Renderpass::SubpassDescription::SubpassDescription(VkPipelineBindPoint bindPoint,
		std::vector<VkAttachmentReference> colorAttachments,
		const std::optional<uint32_t>& depthAttachment)
		: m_ColorAttachments(std::move(colorAttachments))
	{
		m_SubpassDescription.pipelineBindPoint = bindPoint;
		m_SubpassDescription.colorAttachmentCount = (uint32_t)m_ColorAttachments.size();
		m_SubpassDescription.pColorAttachments = m_ColorAttachments.data();

		if (depthAttachment)
		{
			m_DepthStencilAttachment.attachment = *depthAttachment;
			m_DepthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			m_SubpassDescription.pDepthStencilAttachment = &m_DepthStencilAttachment;
		}
	}

	Renderpass::Renderpass(const RenderStage& renderStage, VkFormat depthFormat,
		VkFormat surfaceFormat, VkSampleCountFlagBits samples)
	{
		const LogicalDevice& logicalDevice = Graphics::Get().GetLogicalDevice();

		// Creates the renderpasses attachment descriptions
		std::vector<VkAttachmentDescription> attachmentDescriptions;

		for (const Attachment& attachment : renderStage.GetAttachments())
		{
			auto attachmentSamples = attachment.IsMultisampled() ? samples : VK_SAMPLE_COUNT_1_BIT;

			VkAttachmentDescription desc{};
			desc.samples = attachmentSamples;
			// Clear at beginning of render pass.
			desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			// The image can be read from so it's important to store the attachment results
			desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			// We don't care about initial layout of the attachment.
			desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			switch (attachment.GetType())
			{
			case Attachment::Type::Image:
				desc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				desc.format = attachment.GetFormat();
				break;
			case Attachment::Type::Depth:
				desc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				desc.format = depthFormat;
				break;
			case Attachment::Type::Swapchain:
				desc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				desc.format = surfaceFormat;
				break;
			}

			attachmentDescriptions.emplace_back(desc);
		}

		// Create each subpass and its dependencies
		std::vector<Scope<SubpassDescription>> subpasses;
		std::vector<VkSubpassDependency> dependencies;

		for (const SubpassType& subpassType : renderStage.GetSubpasses())
		{
			// Attachments.
			std::vector<VkAttachmentReference> subpassColorAttachments;

			std::optional<uint32_t> depthAttachment;

			for (uint32_t attachmentBinding : subpassType.GetAttachmentBindings())
			{
				auto attachment = renderStage.GetAttachment(attachmentBinding);

				if (!attachment)
				{
					Log::Error("[Renderpass] Failed to find a renderpass attachment bound to {0}",
						attachmentBinding);
					continue;
				}

				if (attachment->GetType() == Attachment::Type::Depth)
				{
					depthAttachment = attachment->GetBinding();
					continue;
				}

				VkAttachmentReference attachmentReference{};
				attachmentReference.attachment = attachment->GetBinding();
				attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				subpassColorAttachments.emplace_back(attachmentReference);
			}

			// Subpass description.
			subpasses.emplace_back(MakeScope<SubpassDescription>(
				VK_PIPELINE_BIND_POINT_GRAPHICS, subpassColorAttachments, depthAttachment));

			// Subpass dependencies.
			VkSubpassDependency dependency{};
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			dependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			if (subpassType.GetBinding() == renderStage.GetSubpasses().size())
			{
				dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				dependency.srcAccessMask =
					VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			}
			else
			{
				dependency.dstSubpass = subpassType.GetBinding();
			}

			if (subpassType.GetBinding() == 0)
			{
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				dependency.dstAccessMask =
					VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			}
			else
			{
				dependency.srcSubpass = subpassType.GetBinding() - 1;
			}

			dependencies.emplace_back(dependency);
		}


		std::vector<VkSubpassDescription> subpassDescriptions;
		subpassDescriptions.reserve(subpasses.size());

		for (const auto& subpass : subpasses)
		{
			subpassDescriptions.emplace_back(subpass->GetSubpassDescription());
		}

		// Creates the render pass.
		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
		renderPassCreateInfo.pAttachments = attachmentDescriptions.data();
		renderPassCreateInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
		renderPassCreateInfo.pSubpasses = subpassDescriptions.data();
		renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassCreateInfo.pDependencies = dependencies.data();
		RAY_VK_THROW_FAILED(
			vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &m_Renderpass),
			"[Renderpass] Failed to create.");
	}

	Renderpass::~Renderpass()
	{
		const LogicalDevice& device = Graphics::Get().GetLogicalDevice();
		vkDestroyRenderPass(device, m_Renderpass, nullptr);
	}
}  // namespace At0::Ray
