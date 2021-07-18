#include "Rpch.h"
#include "RGraphics.h"

#include "Ray/Utils/RLogger.h"
#include "Ray/Utils/RException.h"

#include "Devices/RWindow.h"
#include "Scene/RScene.h"
#include "Scene/RCamera.h"

#include "Graphics/Core/RRendererInstance.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RSurface.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RSwapchain.h"
#include "RCodex.h"
#include "Core/RResource.h"

#include "Graphics/Commands/RCommandPool.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Commands/RCommandBufferRecorder.h"

#include "Graphics/Images/RDepthImage.h"
#include "Graphics/Images/RImageView.h"

#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/RenderPass/RAttachment.h"
#include "Graphics/RenderPass/RSubpass.h"

#include "Graphics/Buffers/Dynamic/RDynamicVertexBuffer.h"
#include "Graphics/Buffers/Dynamic/RDynamicIndexBuffer.h"
#include "Graphics/Buffers/Dynamic/RDynamicUniformBuffer.h"
#include "Graphics/Buffers/RFramebuffer.h"

#include "Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "Components/RMeshRenderer.h"
#include "Components/RTextRenderer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Core/RRendererLoader.h"

#include "UI/RImGui.h"


#define RAY_MULTITHREADED_COMMAND_BUFFER_RERECORDING 1


namespace At0::Ray
{
	Graphics* Graphics::s_Instance = nullptr;

	Graphics::Graphics()
	{
		if (s_Instance)
			ThrowRuntime("[Graphics] Object already created");

		s_Instance = this;

		UpdateViewport();
		UpdateScissor();

		CreateVulkanObjects();
	}

	void Graphics::CreateVulkanObjects()
	{
		m_RendererInstance = MakeScope<RendererInstance>();
		m_PhysicalDevice = MakeScope<PhysicalDevice>();
		m_Surface = MakeScope<Surface>();
		m_LogicalDevice = MakeScope<LogicalDevice>();

		m_Swapchain = MakeScope<Swapchain>();
		m_CommandPool = MakeScope<CommandPool>(RrCommandPoolCreateResetCommandBuffer);

		m_DepthImage = MakeScope<DepthImage>(
			UInt2{ GetSwapchain().GetExtent().width, GetSwapchain().GetExtent().height });

		CreateRenderPass();
		CreateFramebuffers();
		CreateCommandBuffers();
		CreatePipelineCache();

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
		Attachment colorAttachment(GetSwapchain().GetFormat(), RrImageLayoutPresentSrcKHR,
			Attachment::LoadOp::Clear, Attachment::StoreOp::Store, Attachment::LoadOp::Undefined,
			Attachment::StoreOp::Undefined);

		Attachment depthAttachment(*m_DepthImage, RrImageLayoutDepthStencilAttachment,
			Attachment::LoadOp::Clear, Attachment::StoreOp::Undefined,
			Attachment::LoadOp::Undefined, Attachment::StoreOp::Undefined);

		// Subpass before 0 transforms image from VkAttachmentDescription::initialLayout to
		// VkAttachmentReference::layout

		// Subpass after the last one transforms image from VkAttachmentReference::layout to
		// VkAttachmentDescription::finalLayout

		Subpass subpass;
		subpass.AddColorAttachment(0, colorAttachment);
		subpass.AddDepthAttachment(1, depthAttachment);

		RrSubpassDependency dependency{};

		// Commands recorded in the source subpass are in the source scope
		// Commands recorded in the destination subpass are in the destination scope
		// -->
		//	All commands in the source subpass must at least reach srcStage before any of the
		//	commands in the destination subpass are allowed to start the dstStage of their execution

		// If srcSubpass = VK_SUBPASS_EXTERNAL then the source scope is all commands recorded before
		// vkCmdBeginRenderPass. This means that once subpass 0 starts stages in srcStage must be
		// finished before any commands of the dstSubpass (0) start execution

		// Who is this subpass dependent on?
		dependency.srcSubpass = RR_SUBPASS_EXTERNAL;

		// Who is this subpass?
		dependency.dstSubpass = 0;

		// Stages specified here must be finished once the subpass has started

		// Everything before the renderpass
		// This stage mask mean that before the renderpass starts, all commands in the color
		// attachment output and the early fragment test stage must finish execution.
		dependency.srcStageMask =
			RrPipelineStageColorAttachmentOutput | RrPipelineStageEarlyFragmentTests;

		// Future commands/subpasses need to wait for these stages to finish executing
		// This stage mask means that we wait for the color attachment output and early fragment
		// test stage to finish before recording new commands into these stages
		dependency.dstStageMask =
			RrPipelineStageColorAttachmentOutput | RrPipelineStageEarlyFragmentTests;


		dependency.srcAccessMask = 0;

		// Do we need to write or read from the previous image
		dependency.dstAccessMask =
			RrAccessColorAttachmentWrite | RrAccessDepthStencilAttachmentWrite;

		std::vector<RrAttachmentDescription> attachments;
		attachments.emplace_back(colorAttachment);
		attachments.emplace_back(depthAttachment);

		std::vector<RrSubpassDescription> subpasses;
		subpasses.emplace_back(subpass);

		std::vector<RrSubpassDependency> dependencies;
		dependencies.emplace_back(dependency);

		m_RenderPass = MakeScope<RenderPass>(attachments, subpasses, dependencies);
	}

	void Graphics::CreateFramebuffers()
	{
		m_Framebuffers.resize(GetSwapchain().GetNumberOfImages());
		for (uint32_t i = 0; i < m_Framebuffers.size(); ++i)
		{
			m_Framebuffers[i] = MakeScope<Framebuffer>(
				*m_RenderPass, std::vector<RrImageView>{ *GetSwapchain().GetImageViews()[i],
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

		if (vkCreatePipelineCache(GetDevice(), &createInfo, nullptr,
				(VkPipelineCache*)&m_PipelineCache) != VK_SUCCESS)
			Log::Warn("[Graphics] Failed to create pipeline cache.");
	}

	void Graphics::CreateSyncObjects()
	{
		m_ImagesInFlight.resize(m_Swapchain->GetNumberOfImages(), VK_NULL_HANDLE);

		RrFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.flags = RrFenceCreateSignaled;

		for (uint32_t i = 0; i < s_MaxFramesInFlight; ++i)
		{
			ThrowRenderError(
				RendererAPI::CreateSemaphore(GetDevice(), &m_ImageAvailableSemaphore[i]),
				"[Graphics] Failed to semaphore to signal when an image is avaliable");

			ThrowRenderError(
				RendererAPI::CreateSemaphore(GetDevice(), &m_RenderFinishedSemaphore[i]),
				"[Graphics] Failed to semaphore to signal when an image has finished rendering");

			ThrowRenderError(
				RendererAPI::CreateFence(GetDevice(), &fenceCreateInfo, &m_InFlightFences[i]),
				"[Graphics] Failed to create in flight fence");
		}
	}

	void Graphics::CreateCommandBuffers()
	{
#if RAY_MULTITHREADED_COMMAND_BUFFER_RERECORDING
		m_CommandBufferRecorder =
			MakeScope<CommandBufferRecorder>(std::thread::hardware_concurrency(),
				(uint32_t)m_Framebuffers.size(), *m_RenderPass, 0, m_Framebuffers);
#else

		m_CommandBuffers.resize(m_Framebuffers.size());

		for (uint32_t i = 0; i < m_CommandBuffers.size(); ++i)
		{
			m_CommandBuffers[i] = MakeScope<CommandBuffer>(*m_CommandPool);
		}
#endif
	}

	void Graphics::RecordCommandBuffer(
		const CommandBuffer& cmdBuff, const Framebuffer& framebuffer, uint32_t imageIndex)
	{
		cmdBuff.Begin(RrCommandBufferUsageOneTimeSubmit);

		RrClearValue clearValues[2];
		clearValues[0].color = { 0.0137254f, 0.014117f, 0.0149019f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		m_RenderPass->Begin(cmdBuff, framebuffer, clearValues, std::size(clearValues));

		RendererAPI::CmdSetViewport(cmdBuff, 0, 1, (RrViewport*)&m_Viewport);
		RendererAPI::CmdSetScissor(cmdBuff, 0, 1, (RrRect2D*)&m_Scissor);

		Scene::Get().CmdBind(cmdBuff);
		Scene::Get().EntityView<MeshRenderer>().each(
			[&cmdBuff](MeshRenderer& mesh) { mesh.Render(cmdBuff); });
		Scene::Get().EntityView<TextRenderer>().each(
			[&cmdBuff](TextRenderer& text) { text.Render(cmdBuff); });

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
		RendererAPI::WaitForFences(
			GetDevice(), 1, &m_InFlightFences[m_CurrentFrame], true, UINT64_MAX);

		// Index representing the index of the next image to raw (0-2 in this case)
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(GetDevice(), GetSwapchain(), UINT64_MAX,
			(VkSemaphore)m_ImageAvailableSemaphore[m_CurrentFrame],	 //  Signalled by this function
																	 //  when an image is acquired
			VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			OnFramebufferResized();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			ThrowRuntime("[Graphics] Failed to acquire next swapchain image");

		// Check if previous frame is still using this image (e.g. there is its fence to wait on)
		if (m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
			RendererAPI::WaitForFences(
				GetDevice(), 1, &m_ImagesInFlight[imageIndex], true, UINT64_MAX);

#if RAY_MULTITHREADED_COMMAND_BUFFER_RERECORDING
		// Reset command pool that was used for this frame
		m_CommandBufferRecorder->ResetCommandPools(imageIndex);
#endif

		// Mark the image as now being in use by this frame
		m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrame];

#if RAY_ENABLE_IMGUI
		ImGUI::Get().Update(dt);
		ImGUI::Get().NewFrame();
		ImGUI::Get().UpdateBuffers();
#endif
		Scene::Get().Update(dt);

		RrSubmitInfo submitInfo{};
		RrPipelineStageFlags waitStages[] = { RrPipelineStageColorAttachmentOutput };


#if RAY_MULTITHREADED_COMMAND_BUFFER_RERECORDING
		m_CommandBufferRecorder->Record(*m_RenderPass, *m_Framebuffers[imageIndex], imageIndex,
			*(RrViewport*)&m_Viewport, *(RrRect2D*)&m_Scissor);
		// submitInfo.commandBufferCount =
		//	(uint32_t)m_CommandBufferRecorder->GetVkCommandBuffers(imageIndex).size();
		// submitInfo.pCommandBuffers =
		//	m_CommandBufferRecorder->GetVkCommandBuffers(imageIndex).data();
		submitInfo.commandBufferCount = 1;
		RrCommandBuffer commandBuffer =
			*m_CommandBufferRecorder->GetMainCommandResources(imageIndex).commandBuffer;
		submitInfo.pCommandBuffers = &commandBuffer;
#else
		RecordCommandBuffer(*m_CommandBuffers[imageIndex], *m_Framebuffers[imageIndex], imageIndex);
		RrCommandBuffer commandBuffers[] = { *m_CommandBuffers[imageIndex] };
		submitInfo.commandBufferCount = std::size(commandBuffers);
		submitInfo.pCommandBuffers = commandBuffers;
#endif

		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores =
			(RrSemaphore*)&m_ImageAvailableSemaphore[m_CurrentFrame];  // Wait until image was
																	   // acquired
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores =
			(RrSemaphore*)&m_RenderFinishedSemaphore[m_CurrentFrame];  // Signal when rendering
																	   // finished and presentation
																	   // can happen

		RendererAPI::ResetFences(GetDevice(), 1, &m_InFlightFences[m_CurrentFrame]);

		// Fence will be signaled once the command buffer finishes executing
		ThrowRenderError(RendererAPI::QueueSubmit(GetDevice().GetGraphicsQueue(), 1, &submitInfo,
							 (RrFence)m_InFlightFences[m_CurrentFrame]),
			"[Graphics] Failed to submit image to queue for rendering");

		VkSwapchainKHR swapChain = GetSwapchain();
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = (VkSemaphore*)&m_RenderFinishedSemaphore[m_CurrentFrame];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapChain;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		result = vkQueuePresentKHR((VkQueue)GetDevice().GetPresentQueue(), &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
			m_FramebufferResized)
			OnFramebufferResized();
		else if (result != VK_SUCCESS)
			ThrowRuntime("[Graphics] Failed to present swapchain image");

		m_CurrentFrame = (m_CurrentFrame + 1) % s_MaxFramesInFlight;
	}

	void Graphics::Destroy() { delete s_Instance; }

	Graphics::~Graphics()
	{
		m_LogicalDevice->WaitIdle();

		for (uint32_t i = 0; i < s_MaxFramesInFlight; ++i)
		{
			RendererAPI::DestroySemaphore(*m_LogicalDevice, m_ImageAvailableSemaphore[i]);
			RendererAPI::DestroySemaphore(*m_LogicalDevice, m_RenderFinishedSemaphore[i]);
			RendererAPI::DestroyFence(*m_LogicalDevice, m_InFlightFences[i]);
		}

		m_CommandBuffers.clear();
		m_CommandBufferRecorder.reset();

#if RAY_ENABLE_IMGUI
		ImGUI::Destroy();
#endif
		Scene::Destroy();
		DynamicUniformBuffer::Reset();
		Codex::Shutdown();
		Resources::Destroy();

		if (m_PipelineCache)
		{
			WritePipelineCache();
			vkDestroyPipelineCache(GetDevice(), (VkPipelineCache)m_PipelineCache, nullptr);
		}

		m_DepthImage.reset();
		m_Framebuffers.clear();
		m_RenderPass.reset();

		m_CommandPool.reset();
		m_Swapchain.reset();

		m_LogicalDevice.reset();
		m_Surface.reset();
		m_PhysicalDevice.reset();
		m_RendererInstance.reset();
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
		m_CommandBufferRecorder.reset();
		m_Framebuffers.clear();

		m_RenderPass.reset();
		m_DepthImage.reset();

		m_Swapchain = MakeScope<Swapchain>(m_Swapchain->operator RrSwapchainKHR());
		m_CommandPool.reset();

		m_CommandPool = MakeScope<CommandPool>(RrCommandPoolCreateResetCommandBuffer);
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
		if (vkGetPipelineCacheData(GetDevice(), (VkPipelineCache)m_PipelineCache,
				&pipelineBufferSize, nullptr) != VK_SUCCESS)
			Log::Warn("[Graphics] Failed to get pipeline cache size");

		std::vector<char> pipelineData(pipelineBufferSize);
		if (vkGetPipelineCacheData(GetDevice(), (VkPipelineCache)m_PipelineCache,
				&pipelineBufferSize, pipelineData.data()) != VK_SUCCESS)
			Log::Warn("[Graphics] Failed to get pipeline cache data");

		std::ofstream writer("Resources/Caches/Pipeline.cache", std::ios::binary);
		writer.write(pipelineData.data(), pipelineData.size());
		writer.close();
	}
}  // namespace At0::Ray
