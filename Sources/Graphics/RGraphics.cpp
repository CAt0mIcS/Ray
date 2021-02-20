#include "Rpch.h"
#include "RGraphics.h"

#include "Utils/RLogger.h"
#include "Utils/RException.h"

#include "Devices/RWindow.h"

#include "Graphics/Core/RVulkanInstance.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RSurface.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RSwapchain.h"

#include "Graphics/Commands/RCommandPool.h"
#include "Graphics/Commands/RCommandBuffer.h"

#include "Graphics/Images/RDepthImage.h"
#include "Graphics/Images/RImageView.h"

#include "Graphics/RenderPass/RRenderPass.h"

#include "Graphics/Buffers/RFramebuffer.h"


namespace At0::Ray
{
	Graphics::~Graphics()
	{
		m_LogicalDevice->WaitIdle();

		m_Framebuffers.clear();
		m_RenderPass.reset();

		m_CommandPool.reset();
		m_Swapchain.reset();

		m_LogicalDevice.reset();
		m_Surface.reset();
		m_PhysicalDevice.reset();
		m_VulkanInstance.reset();

		delete s_Instance;
	}

	Graphics& Graphics::Get()
	{
		if (!s_Instance)
			new Graphics();
		return *s_Instance;
	}

	Graphics::Graphics()
	{
		if (s_Instance)
			RAY_THROW_RUNTIME("[Graphics] Object already creaed.");

		s_Instance = this;

		UpdateViewport();
		UpdateScissor();

		CreateVulkanObjects();
	}

	void Graphics::UpdateViewport()
	{
		UInt2 size = Window::Get().GetFramebufferSize();
		m_Viewport.x = 0.0f;
		m_Viewport.y = 0.0f;
		m_Viewport.width = (float)size.x;
		m_Viewport.height = (float)size.y;
		m_Viewport.minDepth = 0.0f;
		m_Viewport.maxDepth = 1.0f;
	}

	void Graphics::UpdateScissor()
	{
		UInt2 size = Window::Get().GetFramebufferSize();
		m_Scissor.offset = { 0, 0 };
		m_Scissor.extent = { (uint32_t)size.x, (uint32_t)size.y };
	}

	void Graphics::CreateVulkanObjects()
	{
		m_VulkanInstance = MakeScope<VulkanInstance>();
		m_PhysicalDevice = MakeScope<PhysicalDevice>();
		m_Surface = MakeScope<Surface>();
		m_LogicalDevice = MakeScope<LogicalDevice>();

		m_Swapchain = MakeScope<Swapchain>();
		m_CommandPool = MakeScope<CommandPool>();
		// m_DepthImage = MakeScope<DepthImage>();

		CreateRenderPass();
		CreateFramebuffers();

		CreateCommandBuffers();
		CreateSyncObjects();
	}

	void Graphics::CreateRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = GetSwapchain().GetFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		std::vector<VkAttachmentDescription> attachments;
		attachments.emplace_back(colorAttachment);

		std::vector<VkSubpassDescription> subpasses;
		subpasses.emplace_back(subpass);

		std::vector<VkSubpassDependency> dependencies;
		dependencies.emplace_back(dependency);

		m_RenderPass = MakeScope<RenderPass>(attachments, subpasses, dependencies);
	}

	void Graphics::CreateFramebuffers()
	{
		m_Framebuffers.reserve(GetSwapchain().GetNumberOfImages());
		for (const Scope<ImageView>& imageView : GetSwapchain().GetImageViews())
		{
			m_Framebuffers.emplace_back(
				MakeScope<Framebuffer>(*m_RenderPass, std::vector<VkImageView>{ *imageView }));
		}
	}

	void Graphics::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(m_Framebuffers.size());

		for (uint32_t i = 0; i < m_CommandBuffers.size(); ++i)
		{
			m_CommandBuffers[i] = MakeScope<CommandBuffer>(*m_CommandPool);

			RecordCommandBuffer(*m_CommandBuffers[i], *m_Framebuffers[i]);
		}
	}

	void Graphics::RecordCommandBuffer(const CommandBuffer& cmdBuff, const Framebuffer& framebuffer)
	{
		cmdBuff.Begin();

		VkClearValue clearColor{ 0.0137254f, 0.014117f, 0.0149019f };
		// VkClearValue depthStencilClearColor{};
		// depthStencilClearColor.depthStencil = { 1.0f, 0 };

		std::vector<VkClearValue> clearValues;
		clearValues.emplace_back(clearColor);
		// clearValues.emplace_back(depthStencilClearColor);

		m_RenderPass->Begin(cmdBuff, framebuffer, clearValues);

		const VkViewport viewports[] = { m_Viewport };
		const VkRect2D scissors[] = { m_Scissor };
		vkCmdSetViewport(cmdBuff, 0, std::size(viewports), viewports);
		vkCmdSetScissor(cmdBuff, 0, std::size(scissors), scissors);

		// m_Model->CmdDraw(cmdBuff);
		// m_Model2->CmdDraw(cmdBuff);
		// for (Scope<Drawable>& drawable : m_Drawables)
		//{
		//	drawable->CmdBind(cmdBuff);
		//	drawable->CmdDraw(cmdBuff);
		//}

		m_RenderPass->End(cmdBuff);

		cmdBuff.End();
	}

	void Graphics::CreateSyncObjects() {}
}  // namespace At0::Ray
