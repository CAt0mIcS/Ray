#include "Rpch.h"
#include "RImGui.h"

#if RAY_ENABLE_IMGUI

	// clang-format off
#include "Devices/RWindow.h"
#include "Devices/RMouse.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RSurface.h"
#include "Graphics/Core/RSwapchain.h"
#include "Graphics/Core/RVulkanInstance.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Utils/RException.h"
#include "Graphics/Buffers/RBuffer.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Images/RImage2D.h"
#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/Images/RTextureSampler.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Pipelines/Uniforms/RSampler2DUniform.h"
#include "Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "Graphics/Images/RTexture2D.h"

#include <../../Extern/imgui/imgui.h>
#include <../../Extern/imgui/imgui_internal.h>
#include <../../Extern/imgui/imgui_impl_glfw.h>
#include <../../Extern/imgui/imgui_impl_vulkan.h>

#ifdef _WIN32
	#define GLFW_EXPOSE_NATIVE_WIN32
#else
	#define GLFW_EXPOSE_NATIVE_X11
#endif

#include <GLFW/glfw3.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <GLFW/glfw3native.h>
// clang-format on

namespace At0::Ray
{
	Scope<ImGUI> ImGUI::s_Instance = nullptr;

	ImGUI& ImGUI::Get()
	{
		if (!s_Instance)
			s_Instance = Scope<ImGUI>(new ImGUI());
		return *s_Instance;
	}

	ImGUI::~ImGUI()
	{
		ImGui::DestroyContext();
		delete m_WindowData;
	}

	static void CheckVkResult(VkResult err) { RAY_VK_THROW_FAILED(err, "[ImGui] Vulkan Error"); }

	ImGUI::ImGUI()
	{
		m_WindowData = new ImGui_ImplVulkanH_Window();
		memset(m_WindowData, 0, sizeof(ImGui_ImplVulkanH_Window));
		m_WindowData->Surface = Graphics::Get().GetSurface();
		m_WindowData->Swapchain = Graphics::Get().GetSwapchain();

		// Select Surface Format
		const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM,
			VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
		const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		m_WindowData->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(
			Graphics::Get().GetPhysicalDevice(), m_WindowData->Surface, requestSurfaceImageFormat,
			(size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR,
			VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
		m_WindowData->PresentMode =
			ImGui_ImplVulkanH_SelectPresentMode(Graphics::Get().GetPhysicalDevice(),
				m_WindowData->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
		// ImGui_ImplVulkanH_CreateOrResizeWindow(Graphics::Get().GetInstance(),
		//	Graphics::Get().GetPhysicalDevice(), Graphics::Get().GetDevice(), m_WindowData,
		//	Graphics::Get().GetDevice().GetGraphicsFamily(), nullptr,
		//	Window::Get().GetFramebufferSize().x, Window::Get().GetFramebufferSize().y, 2);


		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();

		io.DisplaySize =
			ImVec2(Window::Get().GetFramebufferSize().x, Window::Get().GetFramebufferSize().y);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::GetStyle().WindowRounding = 0.0f;
			ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForVulkan(Window::Get().GetNative(), false);

		VkDescriptorPool descriptorPool;
		VkDescriptorPoolSize pool_sizes[] = { { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 }, { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 } };
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		CheckVkResult(vkCreateDescriptorPool(
			Graphics::Get().GetDevice(), &pool_info, nullptr, &descriptorPool));

		ImGui_ImplVulkan_InitInfo vulkanInfo{};
		vulkanInfo.Instance = Graphics::Get().GetInstance();
		vulkanInfo.PhysicalDevice = Graphics::Get().GetPhysicalDevice();
		vulkanInfo.Device = Graphics::Get().GetDevice();
		vulkanInfo.QueueFamily = Graphics::Get().GetDevice().GetGraphicsFamily();
		vulkanInfo.Queue = Graphics::Get().GetDevice().GetGraphicsQueue();
		vulkanInfo.PipelineCache = Graphics::Get().GetPipelineCache();
		vulkanInfo.DescriptorPool = descriptorPool;
		vulkanInfo.Allocator = nullptr;
		vulkanInfo.MinImageCount = 2;
		vulkanInfo.ImageCount = Graphics::GetImageCount();
		vulkanInfo.CheckVkResultFn = CheckVkResult;

		ImGui_ImplVulkan_Init(&vulkanInfo, Graphics::Get().GetRenderPass());

		InitResources();
	}

	void ImGUI::InitResources()
	{
		ImGuiIO& io = ImGui::GetIO();

		// create font texture
		unsigned char* fontData;
		int texWidth, texHeight;
		io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);
		VkDeviceSize uploadSize = texWidth * texHeight * 4 * sizeof(char);

		// Create target image for copy
		m_FontImage = MakeRef<Texture2D>(UInt2{ texWidth, texHeight }, VK_FORMAT_R8G8B8A8_UNORM,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		Buffer stagingBuffer(uploadSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, fontData);

		// Copy
		m_FontImage->TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		m_FontImage->CopyFromBuffer(stagingBuffer);
		m_FontImage->TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		CreatePipeline();
	}

	void ImGUI::NewFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (const auto& fn : m_NewFrameFunctions)
			fn();

		// Render to generate draw buffers
		ImGui::Render();
	}

	void ImGUI::UpdateBuffers()
	{
		ImDrawData* imDrawData = ImGui::GetDrawData();
		VkDeviceSize vertexBufferSize = imDrawData->TotalVtxCount * sizeof(ImDrawVert);
		VkDeviceSize indexBufferSize = imDrawData->TotalIdxCount * sizeof(ImDrawIdx);

		if ((vertexBufferSize == 0) || (indexBufferSize == 0))
			return;

		// Update buffers only if vertex or index count has been changed compared to current buffer
		// size

		// Vertex buffer
		if (!m_VertexBuffer || m_VertexCount != imDrawData->TotalVtxCount)
		{
			if (m_VertexBuffer)
			{
				m_VertexBuffer->UnmapMemory();
				m_VertexBuffer.reset();
			}

			m_VertexBuffer = MakeScope<Buffer>(vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

			m_VertexCount = imDrawData->TotalVtxCount;
			m_VertexBuffer->MapMemory(&m_VertexBufferMapped);
		}

		// Index buffer
		if (!m_IndexBuffer || m_IndexCount < imDrawData->TotalIdxCount)
		{
			if (m_IndexBuffer)
			{
				m_IndexBuffer->UnmapMemory();
				m_IndexBuffer.reset();
			}

			m_IndexBuffer = MakeScope<Buffer>(indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

			m_IndexCount = imDrawData->TotalIdxCount;
			m_IndexBuffer->MapMemory(&m_IndexBufferMapped);
		}

		// Upload data
		ImDrawVert* vtxDst = (ImDrawVert*)m_VertexBufferMapped;
		ImDrawIdx* idxDst = (ImDrawIdx*)m_IndexBufferMapped;

		for (int n = 0; n < imDrawData->CmdListsCount; ++n)
		{
			const ImDrawList* cmdList = imDrawData->CmdLists[n];
			memcpy(vtxDst, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof(ImDrawVert));
			memcpy(idxDst, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx));
			vtxDst += cmdList->VtxBuffer.Size;
			idxDst += cmdList->IdxBuffer.Size;
		}

		// Flush to make writes visible to GPU
		m_VertexBuffer->FlushMemory();
		m_IndexBuffer->FlushMemory();
	}

	void ImGUI::CmdBind(const CommandBuffer& cmdBuff)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImDrawData* drawData = ImGui::GetDrawData();
		const bool windowMinimized =
			(drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
		m_WindowData->ClearValue.color.float32[0] = 0.0f;
		m_WindowData->ClearValue.color.float32[1] = 0.0f;
		m_WindowData->ClearValue.color.float32[2] = 0.0f;
		m_WindowData->ClearValue.color.float32[3] = 1.0f;
		if (!windowMinimized)
		{
			ImGui_ImplVulkan_RenderDrawData(drawData, cmdBuff);
		}

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGUI::Update(Delta dt)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = dt.Change().AsSeconds();
	}

	void ImGUI::CreatePipeline()
	{
		// Vertex bindings an attributes based on ImGui vertex definition
		VkVertexInputBindingDescription vertexInputBinding{};
		vertexInputBinding.binding = 0;
		vertexInputBinding.stride = sizeof(ImDrawVert);
		vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::vector<VkVertexInputAttributeDescription> vertexInputAttributes;
		VkVertexInputAttributeDescription vInputPos{};
		vInputPos.location = 0;
		vInputPos.binding = 0;
		vInputPos.format = VK_FORMAT_R32G32_SFLOAT;
		vInputPos.offset = offsetof(ImDrawVert, pos);
		VkVertexInputAttributeDescription vInputUV{};
		vInputUV.location = 1;
		vInputUV.binding = 0;
		vInputUV.format = VK_FORMAT_R32G32_SFLOAT;
		vInputUV.offset = offsetof(ImDrawVert, uv);
		VkVertexInputAttributeDescription vInputCol{};
		vInputCol.location = 2;
		vInputCol.binding = 0;
		vInputCol.format = VK_FORMAT_R8G8B8A8_UNORM;
		vInputCol.offset = offsetof(ImDrawVert, col);
		vertexInputAttributes.emplace_back(vInputPos);
		vertexInputAttributes.emplace_back(vInputUV);
		vertexInputAttributes.emplace_back(vInputCol);

		GraphicsPipeline::Layout layout;
		layout.shaders = { "Resources/Shaders/ImGui.vert", "Resources/Shaders/ImGui.frag" };
		layout.cullMode = VK_CULL_MODE_NONE;
		layout.depthTestEnabled = false;
		layout.bindingDescriptions = { vertexInputBinding };
		layout.attributeDescriptions = { vertexInputAttributes };

		m_Pipeline = MakeScope<GraphicsPipeline>(std::move(layout));

		m_FontDescriptor = MakeScope<DescriptorSet>(m_Pipeline->GetDescriptorPool(),
			m_Pipeline->GetDescriptorSetLayout(0), Pipeline::BindPoint::Graphics,
			m_Pipeline->GetLayout(), 0);

		m_FontUniform = MakeScope<Sampler2DUniform>("ImGuiFonts", std::move(m_FontImage), 0);
		m_FontDescriptor->BindUniform(*m_FontUniform);
	}

	void ImGUI::OnEvent(FramebufferResizedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetSize().x, e.GetSize().y);
	}

	void ImGUI::OnEvent(MouseMovedEvent& e)
	{
		if (!Window::Get().CursorEnabled())
			return;
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetPos().x, e.GetPos().y);
	}

	void ImGUI::OnEvent(MouseButtonPressedEvent& e)
	{
		if (!Window::Get().CursorEnabled())
			return;
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse)
		{
			switch (e.GetKey())
			{
			case MouseButton::Left: io.MouseDown[0] = true; break;
			case MouseButton::Right: io.MouseDown[1] = true; break;
			case MouseButton::Middle: io.MouseDown[2] = true; break;
			}
			e.Handled = true;
		}
	}

	void ImGUI::OnEvent(MouseButtonReleasedEvent& e)
	{
		if (!Window::Get().CursorEnabled())
			return;
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse)
		{
			switch (e.GetKey())
			{
			case MouseButton::Left: io.MouseDown[0] = false; break;
			case MouseButton::Right: io.MouseDown[1] = false; break;
			case MouseButton::Middle: io.MouseDown[2] = false; break;
			}
			e.Handled = true;
		}
	}

	void ImGUI::OnEvent(KeyPressedEvent& e)
	{
		if (!Window::Get().CursorEnabled())
			return;
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard)
		{
			io.KeysDown[(int)e.GetKey()] = true;
			e.Handled = true;
		}
	}

	void ImGUI::OnEvent(KeyReleasedEvent& e)
	{
		if (!Window::Get().CursorEnabled())
			return;
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard)
		{
			io.KeysDown[(int)e.GetKey()] = false;
			e.Handled = true;
		}
	}

	void ImGUI::OnEvent(CharEvent& e)
	{
		if (!Window::Get().CursorEnabled())
			return;
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantTextInput)
		{
			io.AddInputCharacter(e.GetChar());
			e.Handled = true;
		}
	}

	void ImGUI::OnEvent(ScrollLeftEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += (float)e.GetOffset().x;
	}

	void ImGUI::OnEvent(ScrollRightEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += (float)e.GetOffset().x;
	}

	void ImGUI::OnEvent(ScrollUpEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += (float)e.GetOffset().y;
	}

	void ImGUI::OnEvent(ScrollDownEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += (float)e.GetOffset().y;
	}

	void ImGUI::Float3Widget(std::string_view title, Float3& data)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(title.data());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text(title.data());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		ImGui::Button("X", buttonSize);
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();

		ImGui::DragFloat("##X", &data.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		ImGui::Button("Y", buttonSize);
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();

		ImGui::DragFloat("##Y", &data.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		ImGui::Button("Z", buttonSize);
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();

		ImGui::DragFloat("##Z", &data.z, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

}  // namespace At0::Ray

#endif
