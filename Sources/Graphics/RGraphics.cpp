#include "Rpch.h"
#include "RGraphics.h"

#include "Utils/RLogger.h"
#include "Utils/RException.h"

#include "Devices/RWindow.h"
#include "Core/RScene.h"

#include "Graphics/Core/RVulkanInstance.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RSurface.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RSwapchain.h"
#include "Graphics/Core/RCodex.h"

#include "Graphics/Commands/RCommandPool.h"
#include "Graphics/Commands/RCommandBuffer.h"

#include "Graphics/Images/RDepthImage.h"
#include "Graphics/Images/RImageView.h"

#include "Graphics/RenderPass/RRenderPass.h"

#include "Graphics/Buffers/RBufferSynchronizer.h"
#include "Graphics/Buffers/RFramebuffer.h"

#include "Graphics/Pipelines/RDescriptor.h"
#include "Components/RMesh.h"
#include "Core/RScene.h"

namespace At0::Ray
{
	Scope<DescriptorSet> camDescSet;
	uint32_t cameraUniformOffset;

	Graphics::Graphics()
	{
		if (s_Instance)
			RAY_THROW_RUNTIME("[Graphics] Object already created.");

		s_Instance = this;

		UpdateViewport();
		UpdateScissor();

		CreateVulkanObjects();

		UInt2 size = Window::Get().GetFramebufferSize();
		Camera::Get().SetPosition(glm::vec3(0.0f, 0.0f, -2.5f));
		Camera::Get().SetRotation(glm::vec3(0.0f));
		Camera::Get().SetRotationSpeed(0.07f);
		Camera::Get().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 256.0f);
		Camera::Get().SetMovementSpeed(2.0f);
	}

	void Graphics::CreateVulkanObjects()
	{
		m_VulkanInstance = MakeScope<VulkanInstance>();
		m_PhysicalDevice = MakeScope<PhysicalDevice>();
		m_Surface = MakeScope<Surface>();
		m_LogicalDevice = MakeScope<LogicalDevice>();

		m_Swapchain = MakeScope<Swapchain>();
		m_CommandPool = MakeScope<CommandPool>();
		m_DepthImage = MakeScope<DepthImage>(
			UInt2{ GetSwapchain().GetExtent().width, GetSwapchain().GetExtent().height });

		CreateRenderPass();
		CreateFramebuffers();
		BufferSynchronizer::Create();

		uint32_t minBufferAlignment =
			GetPhysicalDevice().GetProperties().limits.minUniformBufferOffsetAlignment;
		BufferSynchronizer::Get().Emplace(
			sizeof(Matrix) * 2, minBufferAlignment, &cameraUniformOffset);

		VkDescriptorSetLayoutBinding cameraBinding{};
		cameraBinding.binding = 0;
		cameraBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		cameraBinding.descriptorCount = 1;
		cameraBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		cameraBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = 1;
		descriptorSetLayoutCreateInfo.pBindings = &cameraBinding;

		VkDescriptorSetLayout cameraDescSetLayout;
		RAY_VK_THROW_FAILED(vkCreateDescriptorSetLayout(GetDevice(), &descriptorSetLayoutCreateInfo,
								nullptr, &cameraDescSetLayout),
			"[Graphics] Failed to create descriptor set layout for camera.");

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.flags = 0;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &cameraDescSetLayout;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		VkPipelineLayout cameraPipelineLayout;
		RAY_VK_THROW_FAILED(vkCreatePipelineLayout(GetDevice(), &pipelineLayoutCreateInfo, nullptr,
								&cameraPipelineLayout),
			"[Graphics] Failed to create pipeline layout for camera.");

		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = 1;

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.maxSets = 1;
		descriptorPoolCreateInfo.poolSizeCount = 1;
		descriptorPoolCreateInfo.pPoolSizes = &poolSize;

		VkDescriptorPool cameraDescriptorPool;
		RAY_VK_THROW_FAILED(vkCreateDescriptorPool(GetDevice(), &descriptorPoolCreateInfo, nullptr,
								&cameraDescriptorPool),
			"[GraphicsPipeline] Failed to create descriptor pool.");

		camDescSet = MakeScope<DescriptorSet>(cameraDescriptorPool, cameraDescSetLayout,
			Pipeline::BindPoint::Graphics, cameraPipelineLayout,
			DescriptorSet::Frequency::PerScene);


		VkDescriptorBufferInfo bufferInfoCam{};
		bufferInfoCam.buffer =
			BufferSynchronizer::Get().GetUniformBuffer().GetBuffer(cameraUniformOffset);
		bufferInfoCam.offset =
			BufferSynchronizer::Get().GetUniformBuffer().GetOffset(cameraUniformOffset);
		bufferInfoCam.range = sizeof(Matrix) * 2;

		VkWriteDescriptorSet descWriteCam{};
		descWriteCam.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWriteCam.dstSet = *camDescSet;
		descWriteCam.dstBinding = 0;
		descWriteCam.dstArrayElement = 0;
		descWriteCam.descriptorCount = 1;
		descWriteCam.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descWriteCam.pImageInfo = nullptr;
		descWriteCam.pBufferInfo = &bufferInfoCam;
		descWriteCam.pTexelBufferView = nullptr;

		DescriptorSet::Update({ descWriteCam });


		CreateCommandBuffers();
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
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = GetSwapchain().GetFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = m_DepthImage->GetFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
								  VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
								  VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
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
		VkClearValue depthStencilClearColor{};
		depthStencilClearColor.depthStencil = { 1.0f, 0 };

		std::vector<VkClearValue> clearValues;
		clearValues.emplace_back(clearColor);
		clearValues.emplace_back(depthStencilClearColor);

		m_RenderPass->Begin(cmdBuff, framebuffer, clearValues);

		const VkViewport viewports[] = { m_Viewport };
		const VkRect2D scissors[] = { m_Scissor };
		vkCmdSetViewport(cmdBuff, 0, std::size(viewports), viewports);
		vkCmdSetScissor(cmdBuff, 0, std::size(scissors), scissors);

		camDescSet->CmdBind(cmdBuff);
		Scene::Get().EntityView<Mesh>().each([&cmdBuff](Mesh& mesh) {
			mesh.Bind(cmdBuff);
			mesh.Render(cmdBuff);
		});

		m_RenderPass->End(cmdBuff);

		cmdBuff.End();
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
				"[Graphics] Failed to semaphore to signal when an image is avaliable.");

			RAY_VK_THROW_FAILED(vkCreateSemaphore(GetDevice(), &semaphoreCreateInfo, nullptr,
									&m_RenderFinishedSemaphore[i]),
				"[Graphics] Failed to semaphore to signal when an image has finished rendering.");

			RAY_VK_THROW_FAILED(
				vkCreateFence(GetDevice(), &fenceCreateInfo, nullptr, &m_InFlightFences[i]),
				"[Graphics] Failed to create in flight fence.");
		}
	}

	void Graphics::Update(Delta dt)
	{
		Camera::Get().Update(dt.Change().AsSeconds());

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
			RAY_THROW_RUNTIME("[Graphics] Failed to acquire next swapchain image.");

		// Check if previous frame is still using this image (e.g. there is its fence to wait on)
		if (m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(GetDevice(), 1, &m_ImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);

		// Mark the image as now being in use by this frame
		m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrame];

		BufferSynchronizer::Get().Update(Camera::Get().Matrices.View, cameraUniformOffset);
		BufferSynchronizer::Get().Update(
			Camera::Get().Matrices.Perspective, cameraUniformOffset + sizeof(Matrix));

		Scene::Get().Update(dt);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores =
			&m_ImageAvailableSemaphore[m_CurrentFrame];	 // Wait unitl image was acquired
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;

		// RAY_TODO: Multithreaded recording
		m_CommandBuffers[imageIndex] = MakeScope<CommandBuffer>(*m_CommandPool);
		RecordCommandBuffer(*m_CommandBuffers[imageIndex], *m_Framebuffers[imageIndex]);

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
			"[Graphics] Failed to submit image to queue for rendering.");

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
			RAY_THROW_RUNTIME("[Graphics] Failed to present swapchain image.");

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

		BufferSynchronizer::Destroy();
		Codex::Shutdown();

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

	void At0::Ray::Graphics::OnFramebufferResized()
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

		m_Swapchain.reset();
		m_CommandPool.reset();

		m_Swapchain = MakeScope<Swapchain>();
		m_CommandPool = MakeScope<CommandPool>();
		m_DepthImage = MakeScope<DepthImage>(
			UInt2{ GetSwapchain().GetExtent().width, GetSwapchain().GetExtent().height });
		CreateRenderPass();
		UpdateViewport();
		UpdateScissor();
		CreateFramebuffers();
		CreateCommandBuffers();

		size = Window::Get().GetFramebufferSize();
		Camera::Get().UpdateAspectRatio((float)size.x / (float)size.y);

		m_FramebufferResized = false;
	}

	void Graphics::OnEvent(FramebufferResizedEvent& e) { m_FramebufferResized = true; }
}  // namespace At0::Ray
