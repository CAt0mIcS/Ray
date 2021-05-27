#include "Rpch.h"
#include "RGraphics.h"

#include "Utils/RLogger.h"
#include "Utils/RException.h"

#include "Devices/RWindow.h"
#include "Scene/RScene.h"
#include "Scene/RCamera.h"

#include "Graphics/Core/RVulkanInstance.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RSurface.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RSwapchain.h"
#include "RCodex.h"

#include "Graphics/Commands/RCommandPool.h"
#include "Graphics/Commands/RCommandBuffer.h"

#include "Graphics/Images/RDepthImage.h"
#include "Graphics/Images/RImageView.h"

#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/RenderPass/RAttachment.h"
#include "Graphics/RenderPass/RSubpass.h"

#include "Graphics/Buffers/RBufferSynchronizer.h"
#include "Graphics/Buffers/RFramebuffer.h"

#include "Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "Components/RMeshRenderer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"

#include "UI/RImGui.h"


namespace At0::Ray
{
	Graphics* Graphics::s_Instance = nullptr;

	Graphics::Graphics()
	{
		if (s_Instance)
			RAY_THROW_RUNTIME("[Graphics] Object already created");

		s_Instance = this;

		UpdateViewport();
		UpdateScissor();

		CreateVulkanObjects();
	}

	void Graphics::CreateVulkanObjects()
	{
		m_VulkanInstance = MakeScope<VulkanInstance>();
		m_PhysicalDevice = MakeScope<PhysicalDevice>();
		m_Surface = MakeScope<Surface>();
		m_LogicalDevice = MakeScope<LogicalDevice>();

		m_Swapchain = MakeScope<Swapchain>();
		m_CommandPool = MakeScope<CommandPool>(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

		m_DepthImage = MakeScope<DepthImage>(
			UInt2{ GetSwapchain().GetExtent().width, GetSwapchain().GetExtent().height },
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);

		CreateRenderPass();
		CreateFramebuffers();
		CreateCommandBuffers();
		CreatePipelineCache();
		BufferSynchronizer::Create();

		CreateSyncObjects();
	}

	Graphics& Graphics::Get()
	{
		if (!s_Instance)
			new Graphics();
		return *s_Instance;
	}

	void Graphics::CreateRenderPass()
	{
		Attachment colorAttachment(GetSwapchain().GetFormat(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			Attachment::LoadOp::Clear, Attachment::StoreOp::Store, Attachment::LoadOp::Undefined,
			Attachment::StoreOp::Undefined);

		Attachment depthAttachment(*m_DepthImage, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
			Attachment::LoadOp::Clear, Attachment::StoreOp::Undefined,
			Attachment::LoadOp::Undefined, Attachment::StoreOp::Undefined);

		// Subpass before 0 transforms image from VkAttachmentDescription::initialLayout to
		// VkAttachmentReference::layout

		// Subpass after the last one transforms image from VkAttachmentReference::layout to
		// VkAttachmentDescription::finalLayout

		Subpass subpass;
		subpass.AddColorAttachment(0, colorAttachment);
		subpass.AddDepthAttachment(1, depthAttachment);

		VkSubpassDependency dependency{};

		// Commands recorded in the source subpass are in the source scope
		// Commands recorded in the destination subpass are in the destination scope
		// -->
		//	All commands in the source subpass must at least reach srcStage before any of the
		//	commands in the destination subpass are allowed to start the dstStage of their execution

		// If srcSubpass = VK_SUBPASS_EXTERNAL then the source scope is all commands recorded before
		// vkCmdBeginRenderPass. This means that once subpass 0 starts stages in srcStage must be
		// finished before any commands of the dstSubpass (0) start execution

		// Who is this subpass dependent on?
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;

		// Who is this subpass?
		dependency.dstSubpass = 0;

		// Stages specified here must be finished once the subpass has started

		// Everything before the renderpass
		// This stage mask mean that before the renderpass starts, all commands in the color
		// attachment output and the early fragment test stage must finish execution.
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
								  VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

		// Future commands/subpasses need to wait for these stages to finish executing
		// This stage mask means that we wait for the color attachment output and early fragment
		// test stage to finish before recording new commands into these stages
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
								  VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;


		dependency.srcAccessMask = 0;

		// Do we need to write or read from the previous image
		dependency.dstAccessMask =
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::vector<VkAttachmentDescription> attachments;
		attachments.emplace_back(colorAttachment);
		attachments.emplace_back(depthAttachment);

		std::vector<VkSubpassDescription> subpasses;
		subpasses.emplace_back(subpass);

		std::vector<VkSubpassDependency> dependencies;
		dependencies.emplace_back(dependency);

		m_RenderPass = MakeScope<RenderPass>(attachments, subpasses, dependencies);
	}

	void Graphics::CreateFramebuffers()
	{
		m_Framebuffers.resize(GetSwapchain().GetNumberOfImages());
		for (uint32_t i = 0; i < m_Framebuffers.size(); ++i)
		{
			m_Framebuffers[i] = MakeScope<Framebuffer>(
				*m_RenderPass, std::vector<VkImageView>{ *GetSwapchain().GetImageViews()[i],
								   m_DepthImage->GetImageView() });
		}
	}

	void Graphics::CreatePipelineCache()
	{
		std::ifstream reader("Resources/Caches/Pipeline.cache", std::ios::binary | std::ios::ate);
		std::vector<char> data;

		VkPipelineCacheCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

		if (reader.is_open())
		{
			size_t filesize = (size_t)reader.tellg();
			reader.seekg(std::ios::beg);

			data.resize(filesize);
			reader.read(data.data(), filesize);
			reader.close();
			createInfo.initialDataSize = filesize;
			createInfo.pInitialData = data.data();
		}

		if (vkCreatePipelineCache(GetDevice(), &createInfo, nullptr, &m_PipelineCache) !=
			VK_SUCCESS)
			Log::Warn("[Graphics] Failed to create pipeline cache.");
	}

	void Graphics::CreateSyncObjects()
	{
		m_ImagesInFlight.resize(m_Swapchain->GetNumberOfImages(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (uint32_t i = 0; i < s_MaxFramesInFlight; ++i)
		{
			RAY_VK_THROW_FAILED(vkCreateSemaphore(GetDevice(), &semaphoreCreateInfo, nullptr,
									&m_ImageAvailableSemaphore[i]),
				"[Graphics] Failed to semaphore to signal when an image is avaliable");

			RAY_VK_THROW_FAILED(vkCreateSemaphore(GetDevice(), &semaphoreCreateInfo, nullptr,
									&m_RenderFinishedSemaphore[i]),
				"[Graphics] Failed to semaphore to signal when an image has finished rendering");

			RAY_VK_THROW_FAILED(
				vkCreateFence(GetDevice(), &fenceCreateInfo, nullptr, &m_InFlightFences[i]),
				"[Graphics] Failed to create in flight fence");
		}
	}

	void Graphics::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(m_Framebuffers.size());

		for (uint32_t i = 0; i < m_CommandBuffers.size(); ++i)
		{
			m_CommandBuffers[i] = MakeScope<CommandBuffer>(*m_CommandPool);
		}
	}

	void Graphics::RecordCommandBuffer(
		const CommandBuffer& cmdBuff, const Framebuffer& framebuffer, uint32_t imageIndex)
	{
		cmdBuff.Begin();

		VkClearValue clearColor{ 0.0137254f, 0.014117f, 0.0149019f };
		VkClearValue depthStencilClearColor{};
		depthStencilClearColor.depthStencil = { 1.0f, 0 };

		std::vector<VkClearValue> clearValues;
		clearValues.emplace_back(clearColor);
		clearValues.emplace_back(depthStencilClearColor);

#if RAY_ENABLE_IMGUI
		ImGUI::Get().NewFrame();
		ImGUI::Get().UpdateBuffers();
#endif

		m_RenderPass->Begin(cmdBuff, framebuffer, clearValues);

		const VkViewport viewports[] = { m_Viewport };
		const VkRect2D scissors[] = { m_Scissor };
		vkCmdSetViewport(cmdBuff, 0, std::size(viewports), viewports);
		vkCmdSetScissor(cmdBuff, 0, std::size(scissors), scissors);

		Scene::Get().CmdBind(cmdBuff);
		Scene::Get().EntityView<MeshRenderer>().each(
			[&cmdBuff](MeshRenderer& mesh) { mesh.Render(cmdBuff); });

#if RAY_ENABLE_IMGUI
		ImGUI::Get().CmdBind(cmdBuff);
#endif

		m_RenderPass->End(cmdBuff);

		cmdBuff.End();
	}

	void Graphics::Update(Delta dt)
	{
		// Wait for fence in VkQueueSubmit to become signaled
		// which means that the command buffer finished executing
		vkWaitForFences(GetDevice(), 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

		// Index representing the index of the next image to raw (0-2 in this case)
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(GetDevice(), GetSwapchain(), UINT64_MAX,
			m_ImageAvailableSemaphore[m_CurrentFrame],	//  Signalled by this function when an image
														//  is acquired
			VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			OnFramebufferResized();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			RAY_THROW_RUNTIME("[Graphics] Failed to acquire next swapchain image");

		// Check if previous frame is still using this image (e.g. there is its fence to wait on)
		if (m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(GetDevice(), 1, &m_ImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);

		// Mark the image as now being in use by this frame
		m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrame];

#if RAY_ENABLE_IMGUI
		ImGUI::Get().Update(dt);
#endif
		Scene::Get().Update(dt);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores =
			&m_ImageAvailableSemaphore[m_CurrentFrame];	 // Wait until image was acquired
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;

		// RAY_TODO: Multithreaded command buffer rerecording
		RecordCommandBuffer(*m_CommandBuffers[imageIndex], *m_Framebuffers[imageIndex], imageIndex);

		VkCommandBuffer commandBuffer = *m_CommandBuffers[imageIndex];
		submitInfo.pCommandBuffers = &commandBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores =
			&m_RenderFinishedSemaphore[m_CurrentFrame];	 // Signal when rendering finished and
														 // presentation can happen

		vkResetFences(GetDevice(), 1, &m_InFlightFences[m_CurrentFrame]);

		// Fence will be signaled once the command buffer finishes executing
		RAY_VK_THROW_FAILED(vkQueueSubmit(GetDevice().GetGraphicsQueue(), 1, &submitInfo,
								m_InFlightFences[m_CurrentFrame]),
			"[Graphics] Failed to submit image to queue for rendering");

		VkSwapchainKHR swapChain = GetSwapchain();
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_RenderFinishedSemaphore[m_CurrentFrame];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapChain;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		result = vkQueuePresentKHR(GetDevice().GetPresentQueue(), &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
			m_FramebufferResized)
			OnFramebufferResized();
		else if (result != VK_SUCCESS)
			RAY_THROW_RUNTIME("[Graphics] Failed to present swapchain image");

		m_CurrentFrame = (m_CurrentFrame + 1) % s_MaxFramesInFlight;
	}

	void Graphics::Destroy() { delete s_Instance; }

	Graphics::~Graphics()
	{
		m_LogicalDevice->WaitIdle();

		for (uint32_t i = 0; i < s_MaxFramesInFlight; ++i)
		{
			vkDestroySemaphore(*m_LogicalDevice, m_ImageAvailableSemaphore[i], nullptr);
			vkDestroySemaphore(*m_LogicalDevice, m_RenderFinishedSemaphore[i], nullptr);
			vkDestroyFence(*m_LogicalDevice, m_InFlightFences[i], nullptr);
		}

		m_CommandBuffers.clear();

#if RAY_ENABLE_IMGUI
		ImGUI::Destroy();
#endif
		Scene::Destroy();
		BufferSynchronizer::Destroy();
		Codex::Shutdown();

		if (m_PipelineCache)
		{
			WritePipelineCache();
			vkDestroyPipelineCache(GetDevice(), m_PipelineCache, nullptr);
		}

		m_DepthImage.reset();
		m_Framebuffers.clear();
		m_RenderPass.reset();

		m_CommandPool.reset();
		m_Swapchain.reset();

		m_LogicalDevice.reset();
		m_Surface.reset();
		m_PhysicalDevice.reset();
		m_VulkanInstance.reset();
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

	void Graphics::OnFramebufferResized()
	{
		// Minimized window will have framebuffer size of [0 | 0]
		// Sleep until window is back in foreground
		UInt2 size = Window::Get().GetFramebufferSize();
		while (size == UInt2{ 0, 0 })
		{
			size = Window::Get().GetFramebufferSize();
			Window::Get().WaitForEvents();
		}

		m_LogicalDevice->WaitIdle();

		m_CommandBuffers.clear();
		m_Framebuffers.clear();

		m_RenderPass.reset();
		m_DepthImage.reset();

		m_Swapchain = MakeScope<Swapchain>((VkSwapchainKHR)*m_Swapchain);
		m_CommandPool.reset();

		m_CommandPool = MakeScope<CommandPool>(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		m_DepthImage = MakeScope<DepthImage>(
			UInt2{ GetSwapchain().GetExtent().width, GetSwapchain().GetExtent().height });
		CreateRenderPass();
		UpdateViewport();
		UpdateScissor();
		CreateFramebuffers();
		CreateCommandBuffers();
		for (uint32_t i = 0; i < m_CommandBuffers.size(); ++i)
			RecordCommandBuffer(*m_CommandBuffers[i], *m_Framebuffers[i], i);

		size = Window::Get().GetFramebufferSize();
		Scene::Get().GetCamera().UpdateAspectRatio((float)size.x / (float)size.y);

		m_FramebufferResized = false;
	}

	void Graphics::OnEvent(FramebufferResizedEvent& e) { m_FramebufferResized = true; }

	void Graphics::WritePipelineCache()
	{
		size_t pipelineBufferSize;
		if (vkGetPipelineCacheData(GetDevice(), m_PipelineCache, &pipelineBufferSize, nullptr) !=
			VK_SUCCESS)
			Log::Warn("[Graphics] Failed to get pipeline cache size");

		std::vector<char> pipelineData(pipelineBufferSize);
		if (vkGetPipelineCacheData(GetDevice(), m_PipelineCache, &pipelineBufferSize,
				pipelineData.data()) != VK_SUCCESS)
			Log::Warn("[Graphics] Failed to get pipeline cache data");

		std::ofstream writer("Resources/Caches/Pipeline.cache", std::ios::binary);
		writer.write(pipelineData.data(), pipelineData.size());
		writer.close();
	}
}  // namespace At0::Ray
