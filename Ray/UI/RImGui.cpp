#include "Rpch.h"
#include "RImGui.h"

#if RAY_ENABLE_IMGUI

// clang-format off
#include "Devices/RWindow.h"
#include "Devices/RMouse.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RRendererInstance.h"
#include "Graphics/Core/RSurface.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Ray/Utils/RException.h"
#include "Graphics/Buffers/RBuffer.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Images/RImage2D.h"
#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/Images/RTextureSampler.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Pipelines/Shader/RShader.h"
#include "Graphics/Pipelines/Uniforms/RSampler2DUniform.h"
#include "Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "Graphics/Pipelines/Uniforms/RDescriptor.h"
#include "Graphics/Images/RTexture2D.h"
#include "Ray/Utils/RLogger.h"

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
		vkDestroyDescriptorSetLayout(
			Graphics::Get().GetDevice(), m_TextureDescriptorSetLayout, nullptr);
	}

	ImGUI::ImGUI()
	{
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
		CreateTextureUploadResources();
	}

	void ImGUI::NewFrame(std::function<void()> fn)
	{
		ImGui::NewFrame();

	#if RAY_ENABLE_IMGUI_DOCKSPACE
		static bool dockspaceOpen = true;
		// Enable dockspace
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
					   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
					   ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Dockspace", &dockspaceOpen, windowFlags);
		ImGui::PopStyleVar(3);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDragDropFlags_None);
		}
	#endif

		fn();
		for (const auto& func : m_NewFrameFunctions)
			func();

	#if RAY_ENABLE_IMGUI_DOCKSPACE
		ImGui::End();
	#endif

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
				m_VertexBuffer.reset();

			m_VertexBuffer = MakeScope<Buffer>(vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

			m_VertexCount = imDrawData->TotalVtxCount;
		}

		// Index buffer
		if (!m_IndexBuffer || m_IndexCount < imDrawData->TotalIdxCount)
		{
			if (m_IndexBuffer)
				m_IndexBuffer.reset();

			m_IndexBuffer = MakeScope<Buffer>(indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

			m_IndexCount = imDrawData->TotalIdxCount;
		}

		// Upload data
		ImDrawVert* vtxDst = (ImDrawVert*)m_VertexBuffer->GetMapped();
		ImDrawIdx* idxDst = (ImDrawIdx*)m_IndexBuffer->GetMapped();

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
		m_Pipeline->CmdBind(cmdBuff);

		VkViewport viewport{};
		viewport.width = io.DisplaySize.x;
		viewport.height = io.DisplaySize.y;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(cmdBuff, 0, 1, &viewport);

		// UI scale and translate via push constants
		m_PushConstBlock.scale = Float2(2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y);
		m_PushConstBlock.translate = Float2(-1.0f);
		vkCmdPushConstants(cmdBuff, m_Pipeline->GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0,
			sizeof(PushConstBlock), &m_PushConstBlock);

		// Render commands
		ImDrawData* imDrawData = ImGui::GetDrawData();
		int32_t vertexOffset = 0;
		int32_t indexOffset = 0;

		if (imDrawData->CmdListsCount > 0)
		{
			VkDeviceSize offsets[1] = { 0 };
			VkBuffer vBuff = (VkBuffer)m_VertexBuffer->GetBuffer();
			vkCmdBindVertexBuffers(cmdBuff, 0, 1, &vBuff, offsets);
			vkCmdBindIndexBuffer(
				cmdBuff, (VkBuffer)m_IndexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

			for (int32_t i = 0; i < imDrawData->CmdListsCount; i++)
			{
				const ImDrawList* cmd_list = imDrawData->CmdLists[i];
				for (int32_t j = 0; j < cmd_list->CmdBuffer.Size; j++)
				{
					const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[j];
					VkRect2D scissorRect;
					scissorRect.offset.x = std::max((int32_t)(pcmd->ClipRect.x), 0);
					scissorRect.offset.y = std::max((int32_t)(pcmd->ClipRect.y), 0);
					scissorRect.extent.width = (uint32_t)(pcmd->ClipRect.z - pcmd->ClipRect.x);
					scissorRect.extent.height = (uint32_t)(pcmd->ClipRect.w - pcmd->ClipRect.y);

					// Bind descriptorset with font or user texture
					VkDescriptorSet descSet[1] = { (VkDescriptorSet)pcmd->TextureId };
					vkCmdBindDescriptorSets(cmdBuff, VK_PIPELINE_BIND_POINT_GRAPHICS,
						m_Pipeline->GetLayout(), 0, 1, descSet, 0, nullptr);

					vkCmdSetScissor(cmdBuff, 0, 1, &scissorRect);
					vkCmdDrawIndexed(cmdBuff, pcmd->ElemCount, 1, indexOffset, vertexOffset, 0);
					indexOffset += pcmd->ElemCount;
				}
				vertexOffset += cmd_list->VtxBuffer.Size;
			}
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
		layout.shader = MakeRef<Shader>(std::vector<std::string>{
			"Resources/Shaders/ImGui.vert", "Resources/Shaders/ImGui.frag" });
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

		// Set font id for default font to the descriptor set which should be bound when rendering
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts[0].SetTexID((ImTextureID)*m_FontDescriptor);
	}

	void ImGUI::CreateTextureUploadResources()
	{
		// Descriptor set layout
		{
			VkDescriptorSetLayoutBinding binding[1] = {};
			binding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			binding[0].descriptorCount = 1;
			binding[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			VkDescriptorSetLayoutCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			info.bindingCount = 1;
			info.pBindings = binding;
			ThrowRenderError(vkCreateDescriptorSetLayout(Graphics::Get().GetDevice(), &info,
								 nullptr, &m_TextureDescriptorSetLayout),
				"[ImGUI] Failed to create descriptor set layout for texture upload");
		}
	}

	void* ImGUI::PushTexture(const Texture2D& texture)
	{
		return PushTexture(texture.GetSampler(), texture.GetImageView(), texture.GetImageLayout());
	}

	void* ImGUI::PushTexture(VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout)
	{
		// Descriptor set
		{
			m_TextureDescriptorSets.emplace_back();
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = m_Pipeline->GetDescriptorPool();
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &m_TextureDescriptorSetLayout;
			ThrowRenderError(vkAllocateDescriptorSets(Graphics::Get().GetDevice(), &allocInfo,
								 &m_TextureDescriptorSets.back()),
				"[ImGUI] Failed to create texture descriptor set");
		}

		// Update the Descriptor Set:
		{
			VkDescriptorImageInfo descImage[1] = {};
			descImage[0].sampler = sampler;
			descImage[0].imageView = imageView;
			descImage[0].imageLayout = imageLayout;
			VkWriteDescriptorSet writeDesc[1] = {};
			writeDesc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeDesc[0].dstSet = m_TextureDescriptorSets.back();
			writeDesc[0].descriptorCount = 1;
			writeDesc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeDesc[0].pImageInfo = descImage;
			vkUpdateDescriptorSets(Graphics::Get().GetDevice(), 1, writeDesc, 0, NULL);
		}

		return (ImTextureID)m_TextureDescriptorSets.back();
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

	Float3 ImGUI::Float3Widget(std::string_view title, Float3 data)
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

		return data;
	}

}  // namespace At0::Ray

#endif
