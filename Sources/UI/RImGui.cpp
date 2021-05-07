#include "Rpch.h"
#include "RImGui.h"

#include "Devices/RWindow.h"
#include "Devices/RMouse.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Utils/RException.h"
#include "Graphics/Buffers/RBuffer.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Images/RImage2D.h"
#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/Images/RTextureSampler.h"

#include <../../Extern/imgui/imgui.h>


namespace At0::Ray
{
#pragma region TestBuffer
	VkResult GuiBuffer::map(VkDeviceSize size, VkDeviceSize offset)
	{
		return vkMapMemory(Graphics::Get().GetDevice(), memory, offset, size, 0, &mapped);
	}

	void GuiBuffer::unmap()
	{
		if (mapped)
		{
			vkUnmapMemory(Graphics::Get().GetDevice(), memory);
			mapped = nullptr;
		}
	}

	VkResult GuiBuffer::bind(VkDeviceSize offset)
	{
		return vkBindBufferMemory(Graphics::Get().GetDevice(), buffer, memory, offset);
	}

	void GuiBuffer::setupDescriptor(VkDeviceSize size, VkDeviceSize offset)
	{
		descriptor.offset = offset;
		descriptor.buffer = buffer;
		descriptor.range = size;
	}

	void GuiBuffer::copyTo(void* data, VkDeviceSize size)
	{
		assert(mapped);
		memcpy(mapped, data, size);
	}

	VkResult GuiBuffer::flush(VkDeviceSize size, VkDeviceSize offset)
	{
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkFlushMappedMemoryRanges(Graphics::Get().GetDevice(), 1, &mappedRange);
	}

	VkResult GuiBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
	{
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkInvalidateMappedMemoryRanges(Graphics::Get().GetDevice(), 1, &mappedRange);
	}

	void GuiBuffer::destroy()
	{
		if (buffer)
		{
			vkDestroyBuffer(Graphics::Get().GetDevice(), buffer, nullptr);
		}
		if (memory)
		{
			vkFreeMemory(Graphics::Get().GetDevice(), memory, nullptr);
		}
	}
#pragma endregion

	ImGUI& ImGUI::Get()
	{
		static ImGUI instance;
		return instance;
	}

	void ImGUI::Destroy()
	{
		// RAY_TODO: ImGui flickering if imgui.ini file exists (created when context is destroyed)
		ImGui::DestroyContext();

		// Release all Vulkan resources required for rendering imGui
		Get().m_VertexBuffer.destroy();
		Get().m_IndexBuffer.destroy();
		Get().m_FontImage.reset();
		Get().m_Sampler.reset();
		for (VkShaderModule shaderModule : Get().m_ShaderModules)
			vkDestroyShaderModule(Graphics::Get().GetDevice(), shaderModule, nullptr);
		vkDestroyPipeline(Graphics::Get().GetDevice(), Get().m_Pipeline, nullptr);
		vkDestroyPipelineLayout(Graphics::Get().GetDevice(), Get().m_PipelineLayout, nullptr);
		vkDestroyDescriptorPool(Graphics::Get().GetDevice(), Get().m_DescriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(
			Graphics::Get().GetDevice(), Get().m_DescriptorSetLayout, nullptr);
	}

	ImGUI::ImGUI()
	{
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;
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
		m_FontImage = MakeScope<Image2D>(UInt2{ texWidth, texHeight }, VK_FORMAT_R8G8B8A8_UNORM,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		Buffer stagingBuffer(uploadSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, fontData);

		m_FontImage->TransitionLayout(
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		// Copy
		m_FontImage->CopyFromBuffer(stagingBuffer);

		m_FontImage->TransitionLayout(
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// Font texture sampler
		m_Sampler = MakeScope<TextureSampler>(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
			VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

		CreatePipeline();
	}

	void ImGUI::NewFrame()
	{
		ImGui::NewFrame();

		// Init imGui windows and elements
		ImVec4 clear_color = ImColor(114, 144, 154);
		static float f = 0.0f;
		ImGui::TextUnformatted("Window Title Here");
		ImGui::TextUnformatted(Graphics::Get().GetPhysicalDevice().GetProperties().deviceName);

		ImGui::Text("Camera");
		ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiSetCond_FirstUseEver);

		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowDemoWindow();

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
		if ((m_VertexBuffer.buffer == VK_NULL_HANDLE) ||
			(m_VertexCount != imDrawData->TotalVtxCount))
		{
			m_VertexBuffer.unmap();
			m_VertexBuffer.destroy();
			Buffer::CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, m_VertexBuffer.buffer, m_VertexBuffer.memory);
			m_VertexCount = imDrawData->TotalVtxCount;
			m_VertexBuffer.bind();
			m_VertexBuffer.map();
		}

		// Index buffer
		if ((m_IndexBuffer.buffer == VK_NULL_HANDLE) || (m_IndexCount < imDrawData->TotalIdxCount))
		{
			m_IndexBuffer.unmap();
			m_IndexBuffer.destroy();
			Buffer::CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, m_IndexBuffer.buffer, m_IndexBuffer.memory);
			m_IndexCount = imDrawData->TotalIdxCount;
			m_IndexBuffer.bind();
			m_IndexBuffer.map();
		}

		// Upload data
		ImDrawVert* vtxDst = (ImDrawVert*)m_VertexBuffer.mapped;
		ImDrawIdx* idxDst = (ImDrawIdx*)m_IndexBuffer.mapped;

		for (int n = 0; n < imDrawData->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = imDrawData->CmdLists[n];
			memcpy(vtxDst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
			memcpy(idxDst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
			vtxDst += cmd_list->VtxBuffer.Size;
			idxDst += cmd_list->IdxBuffer.Size;
		}

		// Flush to make writes visible to GPU
		m_VertexBuffer.flush();
		m_IndexBuffer.flush();
	}

	void ImGUI::DrawFrame(VkCommandBuffer commandBuffer)
	{
		ImGuiIO& io = ImGui::GetIO();
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0,
			1, &m_DescriptorSet, 0, nullptr);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);

		VkViewport viewport{};
		viewport.width = io.DisplaySize.x;
		viewport.height = io.DisplaySize.y;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		// UI scale and translate via push constants
		m_PushConstBlock.scale = Float2(2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y);
		m_PushConstBlock.translate = Float2(-1.0f);
		vkCmdPushConstants(commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0,
			sizeof(PushConstBlock), &m_PushConstBlock);

		// Render commands
		ImDrawData* imDrawData = ImGui::GetDrawData();
		int32_t vertexOffset = 0;
		int32_t indexOffset = 0;

		if (imDrawData->CmdListsCount > 0)
		{
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &m_VertexBuffer.buffer, offsets);
			vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);

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
					vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);
					vkCmdDrawIndexed(
						commandBuffer, pcmd->ElemCount, 1, indexOffset, vertexOffset, 0);
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
		// Shader stages
		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};
		{
			std::string shader = "Resources/Shaders/ImGui.vert";
			VkShaderStageFlagBits stageFlag = Shader::GetShaderStage(shader);
			std::optional<std::string> shaderCode = ReadFile(shader);

			VkShaderModule shaderModule = m_Shader.CreateShaderModule(
				shader, *shaderCode, /*defineBlock.str()*/ "", stageFlag);

			VkPipelineShaderStageCreateInfo shaderStageCreateInfo{};
			shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageCreateInfo.stage = stageFlag;
			shaderStageCreateInfo.module = shaderModule;
			shaderStageCreateInfo.pName = "main";
			shaderStages[0] = shaderStageCreateInfo;
		}
		{
			std::string shader = "Resources/Shaders/ImGui.frag";
			VkShaderStageFlagBits stageFlag = Shader::GetShaderStage(shader);
			std::optional<std::string> shaderCode = ReadFile(shader);

			VkShaderModule shaderModule = m_Shader.CreateShaderModule(
				shader, *shaderCode, /*defineBlock.str()*/ "", stageFlag);

			VkPipelineShaderStageCreateInfo shaderStageCreateInfo{};
			shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageCreateInfo.stage = stageFlag;
			shaderStageCreateInfo.module = shaderModule;
			shaderStageCreateInfo.pName = "main";
			shaderStages[1] = shaderStageCreateInfo;
		}

		// Descriptor pool
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSize.descriptorCount = 1;

		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(1);
		descriptorPoolInfo.pPoolSizes = &poolSize;
		descriptorPoolInfo.maxSets = 2;
		RAY_VK_THROW_FAILED(vkCreateDescriptorPool(Graphics::Get().GetDevice(), &descriptorPoolInfo,
								nullptr, &m_DescriptorPool),
			"[ImGui] Failed to create sampler descriptor pool");

		// Descriptor set layout
		VkDescriptorSetLayoutBinding setLayoutBinding{};
		setLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		setLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		setLayoutBinding.binding = 0;
		setLayoutBinding.descriptorCount = 1;

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.pBindings = &setLayoutBinding;
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(1);
		RAY_VK_THROW_FAILED(vkCreateDescriptorSetLayout(Graphics::Get().GetDevice(),
								&descriptorSetLayoutCreateInfo, nullptr, &m_DescriptorSetLayout),
			"[ImGui] Failed to create descriptor set layout");

		// Descriptor set
		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.descriptorPool = m_DescriptorPool;
		descriptorSetAllocateInfo.pSetLayouts = &m_DescriptorSetLayout;
		descriptorSetAllocateInfo.descriptorSetCount = 1;

		RAY_VK_THROW_FAILED(vkAllocateDescriptorSets(Graphics::Get().GetDevice(),
								&descriptorSetAllocateInfo, &m_DescriptorSet),
			"[ImGui] Failed to allocate descriptor set");

		VkDescriptorImageInfo fontDescriptor{};
		fontDescriptor.sampler = *m_Sampler;
		fontDescriptor.imageView = m_FontImage->GetImageView();
		fontDescriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		std::vector<VkWriteDescriptorSet> writeDescriptorSets;
		VkWriteDescriptorSet writeDescriptorSet{};
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.dstSet = m_DescriptorSet;
		writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeDescriptorSet.dstBinding = 0;
		writeDescriptorSet.pImageInfo = &fontDescriptor;
		writeDescriptorSet.descriptorCount = 1;
		writeDescriptorSets.emplace_back(writeDescriptorSet);
		vkUpdateDescriptorSets(Graphics::Get().GetDevice(),
			static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0,
			nullptr);

		// Pipeline layout
		// Push constants for UI rendering parameters
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstBlock);

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
		pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;
		RAY_VK_THROW_FAILED(vkCreatePipelineLayout(Graphics::Get().GetDevice(),
								&pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout),
			"[ImGui] Failed to create pipeline layout");

		// Setup graphics pipeline for UI rendering
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
		inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyState.flags = 0;
		inputAssemblyState.primitiveRestartEnable = VK_FALSE;

		VkPipelineRasterizationStateCreateInfo rasterizationState{};
		rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationState.cullMode = VK_CULL_MODE_NONE;
		rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationState.flags = 0;
		rasterizationState.depthClampEnable = VK_FALSE;
		rasterizationState.lineWidth = 1.0f;

		// Enable blending
		VkPipelineColorBlendAttachmentState blendAttachmentState{};
		blendAttachmentState.blendEnable = VK_TRUE;
		blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
											  VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
		blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlendState{};
		colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendState.attachmentCount = 1;
		colorBlendState.pAttachments = &blendAttachmentState;

		VkPipelineDepthStencilStateCreateInfo depthStencilState{};
		depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilState.depthTestEnable = VK_FALSE;
		depthStencilState.depthWriteEnable = VK_FALSE;
		depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;
		viewportState.flags = 0;

		VkPipelineMultisampleStateCreateInfo multisampleState{};
		multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampleState.flags = 0;

		std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.pDynamicStates = dynamicStateEnables.data();
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
		dynamicState.flags = 0;

		VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.layout = m_PipelineLayout;
		pipelineCreateInfo.renderPass = Graphics::Get().GetRenderPass();
		pipelineCreateInfo.flags = 0;
		pipelineCreateInfo.basePipelineIndex = -1;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

		pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
		pipelineCreateInfo.pRasterizationState = &rasterizationState;
		pipelineCreateInfo.pColorBlendState = &colorBlendState;
		pipelineCreateInfo.pMultisampleState = &multisampleState;
		pipelineCreateInfo.pViewportState = &viewportState;
		pipelineCreateInfo.pDepthStencilState = &depthStencilState;
		pipelineCreateInfo.pDynamicState = &dynamicState;
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
		pipelineCreateInfo.pStages = shaderStages.data();

		// Vertex bindings an attributes based on ImGui vertex definition
		VkVertexInputBindingDescription vertexInputBinding{};
		vertexInputBinding.binding = 0;
		vertexInputBinding.stride = sizeof(ImDrawVert);
		vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::array<VkVertexInputAttributeDescription, 3> vertexInputAttributes;
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
		vertexInputAttributes[0] = vInputPos;
		vertexInputAttributes[1] = vInputUV;
		vertexInputAttributes[2] = vInputCol;

		VkPipelineVertexInputStateCreateInfo vertexInputState{};
		vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputState.vertexBindingDescriptionCount = 1;
		vertexInputState.pVertexBindingDescriptions = &vertexInputBinding;
		vertexInputState.vertexAttributeDescriptionCount = (uint32_t)vertexInputAttributes.size();
		vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes.data();

		pipelineCreateInfo.pVertexInputState = &vertexInputState;

		RAY_VK_THROW_FAILED(vkCreateGraphicsPipelines(Graphics::Get().GetDevice(), nullptr, 1,
								&pipelineCreateInfo, nullptr, &m_Pipeline),
			"[ImGui] Failed to create pipeline");

		for (const auto& shaderStage : shaderStages)
			m_ShaderModules.emplace_back(shaderStage.module);
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

		switch (e.GetKey())
		{
		case MouseButton::Left: io.MouseDown[0] = true; break;
		case MouseButton::Right: io.MouseDown[1] = true; break;
		}
	}

	void ImGUI::OnEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();

		switch (e.GetKey())
		{
		case MouseButton::Left: io.MouseDown[0] = false; break;
		case MouseButton::Right: io.MouseDown[1] = false; break;
		}
	}

	void ImGUI::OnEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard)
		{
			io.KeysDown[(int)e.GetKey()] = true;
		}
	}

	void ImGUI::OnEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard)
		{
			io.KeysDown[(int)e.GetKey()] = false;
		}
	}

	void ImGUI::OnEvent(CharEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantTextInput)
			io.AddInputCharacter(e.GetChar());
	}

	void ImGUI::OnEvent(ScrollLeftEvent& e) {}

	void ImGUI::OnEvent(ScrollRightEvent& e) {}

	void ImGUI::OnEvent(ScrollUpEvent& e) {}

	void ImGUI::OnEvent(ScrollDownEvent& e) {}

}  // namespace At0::Ray
