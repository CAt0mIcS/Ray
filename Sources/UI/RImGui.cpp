#include "Rpch.h"
#include "RImGui.h"

#if RAY_ENABLE_IMGUI

// clang-format off
#include "Devices/RWindow.h"
#include "Devices/RMouse.h"

#include "Graphics/RGraphics.h"
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
#include "Graphics/Pipelines/Uniforms/RSamplerUniform.h"
#include "Graphics/Images/RTexture2D.h"

#include <../../Extern/imgui/imgui.h>
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

	ImGUI::~ImGUI() { ImGui::DestroyContext(); }

	ImGUI::ImGUI()
	{
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();

		io.DisplaySize =
			ImVec2(Window::Get().GetFramebufferSize().x, Window::Get().GetFramebufferSize().y);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		MapKeySpace();
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
		m_Pipeline->CmdBind(cmdBuff);
		m_FontUniform->CmdBind(cmdBuff);

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
			VkBuffer vBuff = *m_VertexBuffer;
			vkCmdBindVertexBuffers(cmdBuff, 0, 1, &vBuff, offsets);
			vkCmdBindIndexBuffer(cmdBuff, *m_IndexBuffer, 0, VK_INDEX_TYPE_UINT16);

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

		m_Pipeline = MakeScope<GraphicsPipeline>(Graphics::Get().GetRenderPass(),
			std::vector<std::string>{
				"Resources/Shaders/ImGui.vert", "Resources/Shaders/ImGui.frag" },
			nullptr, nullptr, VK_CULL_MODE_NONE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VK_POLYGON_MODE_FILL, 1.0f, false, std::vector{ vertexInputBinding },
			vertexInputAttributes);

		m_FontUniform = MakeScope<SamplerUniform>("ImGuiFonts",
			m_Pipeline->GetDescriptorSetLayout(0), m_Pipeline->GetDescriptorPool(),
			Pipeline::BindPoint::Graphics, m_Pipeline->GetLayout(), 0, std::move(m_FontImage));
	}

	void ImGUI::MapKeySpace()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
	}

	void ImGUI::OnEvent(FramebufferResizedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetSize().x, e.GetSize().y);
	}

	void ImGUI::OnEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetPos().x, e.GetPos().y);
	}

	void ImGUI::OnEvent(MouseButtonPressedEvent& e)
	{
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
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard)
		{
			io.KeysDown[(int)e.GetKey()] = true;
			e.Handled = true;
		}
	}

	void ImGUI::OnEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard)
		{
			io.KeysDown[(int)e.GetKey()] = false;
			e.Handled = true;
		}
	}

	void ImGUI::OnEvent(CharEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantTextInput)
		{
			io.AddInputCharacter(e.GetChar());
			e.Handled = true;
		}
	}

	void ImGUI::OnEvent(ScrollLeftEvent& e) {}

	void ImGUI::OnEvent(ScrollRightEvent& e) {}

	void ImGUI::OnEvent(ScrollUpEvent& e) {}

	void ImGUI::OnEvent(ScrollDownEvent& e) {}

}  // namespace At0::Ray

#endif
