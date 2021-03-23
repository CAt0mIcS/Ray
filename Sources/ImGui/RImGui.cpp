#include "Rpch.h"
#include "RImGui.h"

#if RAY_USE_IMGUI

// clang-format off

#include "Devices/RWindow.h"
#include "Graphics/RGraphics.h"

#include "Graphics/Core/RVulkanInstance.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RSwapchain.h"
#include "Graphics/Pipelines/RPipeline.h"
#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Core/RSurface.h"

#include "Utils/RException.h"
#include "Utils/RAssert.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_vulkan.h>
// clang-format on

namespace At0::Ray
{
	Scope<ImGuiContext> ImGuiContext::s_Instance = nullptr;

	static void ImGuiVkError(VkResult error) { RAY_VK_THROW_FAILED(error, "[ImGui] Vulkan error"); }

	ImGuiContext::ImGuiContext()
	{
		RAY_MEXPECTS(!s_Instance, "[ImGuiContext] Already created");
		s_Instance = Scope<ImGuiContext>(this);

		CreateDescriptorPool();
		CreateRenderPass();

		SetupVulkanWindow();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForVulkan(Window::Get().GetNative(), true);


		ImGui_ImplVulkan_InitInfo initInfo{};
		initInfo.Instance = Graphics::Get().GetInstance();
		initInfo.PhysicalDevice = Graphics::Get().GetPhysicalDevice();
		initInfo.Device = Graphics::Get().GetDevice();
		initInfo.QueueFamily = Graphics::Get().GetDevice().GetGraphicsFamily();
		initInfo.Queue = Graphics::Get().GetDevice().GetGraphicsQueue();
		initInfo.PipelineCache = nullptr;
		initInfo.DescriptorPool = m_DescriptorPool;
		initInfo.Allocator = nullptr;
		initInfo.MinImageCount =
			Graphics::Get().GetSwapchain().QuerySwapchainSupport().Capabilities.minImageCount;
		initInfo.ImageCount = Graphics::Get().GetSwapchain().GetNumberOfImages();
		initInfo.CheckVkResultFn = ImGuiVkError;
		ImGui_ImplVulkan_Init(&initInfo, m_RenderPass);

		UploadFonts();
	}

	ImGuiContext::~ImGuiContext()
	{
		vkDestroyDescriptorPool(Graphics::Get().GetDevice(), m_DescriptorPool, nullptr);
		vkDestroyRenderPass(Graphics::Get().GetDevice(), m_RenderPass, nullptr);
	}

	ImGuiContext& ImGuiContext::Get() { return *s_Instance; }

	void ImGuiContext::OnFramebufferResized()
	{
		ImGui_ImplVulkan_SetMinImageCount(
			Graphics::Get().GetSwapchain().QuerySwapchainSupport().Capabilities.minImageCount);
		ImGui_ImplVulkanH_CreateOrResizeWindow(Graphics::Get().GetInstance(),
			Graphics::Get().GetPhysicalDevice(), Graphics::Get().GetDevice(), m_WindowData.get(),
			Graphics::Get().GetDevice().GetGraphicsFamily(), nullptr,
			Window::Get().GetFramebufferSize().x, Window::Get().GetFramebufferSize().y,
			Graphics::Get().GetSwapchain().QuerySwapchainSupport().Capabilities.minImageCount);
		m_WindowData->FrameIndex = 0;
	}

	void ImGuiContext::NewFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bool open = true;
		ImGui::ShowDemoWindow(&open);

		ImGui::Render();
	}

	ImGuiContext& ImGuiContext::Create()
	{
		new ImGuiContext();
		return *s_Instance;
	}

	void ImGuiContext::Destroy() { s_Instance.reset(); }

	void ImGuiContext::SetupVulkanWindow()
	{
		//	m_WindowData = MakeScope<ImGui_ImplVulkanH_Window>();
		//	m_WindowData->Surface = Graphics::Get().GetSurface();
		//	m_WindowData->Swapchain = Graphics::Get().GetSwapchain();

		//	// Select Surface Format
		//	const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM,
		//		VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
		//	const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		//	m_WindowData->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(
		//		Graphics::Get().GetPhysicalDevice(), m_WindowData->Surface,
		//requestSurfaceImageFormat, 		(size_t)IM_ARRAYSIZE(requestSurfaceImageFormat),
		//requestSurfaceColorSpace);

		//	// Select Present Mode
		//#ifdef IMGUI_UNLIMITED_FRAME_RATE
		//	VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR,
		//		VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
		//#else
		//	VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
		//#endif
		//	m_WindowData->PresentMode =
		//		ImGui_ImplVulkanH_SelectPresentMode(Graphics::Get().GetPhysicalDevice(),
		//			m_WindowData->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));

		//	ImGui_ImplVulkanH_CreateOrResizeWindow(Graphics::Get().GetInstance(),
		//		Graphics::Get().GetPhysicalDevice(), Graphics::Get().GetDevice(),
		//m_WindowData.get(), 		Graphics::Get().GetDevice().GetGraphicsFamily(), nullptr,
		//		Window::Get().GetFramebufferSize().x, Window::Get().GetFramebufferSize().y,
		//		Graphics::Get().GetSwapchain().QuerySwapchainSupport().Capabilities.minImageCount);
	}

	void ImGuiContext::CreateDescriptorPool()
	{
		VkDescriptorPoolSize poolSizes[] = { { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 }, { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 } };

		VkDescriptorPoolCreateInfo descPoolCreateInfo{};
		descPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descPoolCreateInfo.maxSets = 16;
		descPoolCreateInfo.poolSizeCount = std::size(poolSizes);
		descPoolCreateInfo.pPoolSizes = poolSizes;

		RAY_VK_THROW_FAILED(vkCreateDescriptorPool(Graphics::Get().GetDevice(), &descPoolCreateInfo,
								nullptr, &m_DescriptorPool),
			"[ImGui] Failed to create descriptor pool");
	}

	void ImGuiContext::CreateRenderPass()
	{
		VkAttachmentDescription attachment = {};
		attachment.format = Graphics::Get().GetSwapchain().GetFormat();
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachment = {};
		colorAttachment.attachment = 0;
		colorAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachment;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &attachment;
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpass;
		createInfo.dependencyCount = 1;
		createInfo.pDependencies = &dependency;
		RAY_VK_THROW_FAILED(
			vkCreateRenderPass(Graphics::Get().GetDevice(), &createInfo, nullptr, &m_RenderPass),
			"[ImGui] Failed to create render pass");
	}

	void ImGuiContext::UploadFonts()
	{
		CommandBuffer cmdBuff(Graphics::Get().GetCommandPool());
		cmdBuff.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		ImGui_ImplVulkan_CreateFontsTexture(cmdBuff);

		cmdBuff.End();

		VkCommandBuffer cmdBuffer = cmdBuff;
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuffer;

		RAY_VK_THROW_FAILED(vkQueueSubmit(Graphics::Get().GetDevice().GetGraphicsQueue(), 1,
								&submitInfo, VK_NULL_HANDLE),
			"[ImGui] Failed to upload fonts to GPU");
		vkQueueWaitIdle(Graphics::Get().GetDevice().GetGraphicsQueue());
		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}
}  // namespace At0::Ray

#endif
