#include "Rpch.h"
#include "RMesh.h"

#include <numeric>

#include "Graphics/RGraphics.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Core/RCodex.h"

#include "Core/RVertex.h"
#include "Utils/RException.h"
#include "Core/RScene.h"

#include "Graphics/Pipelines/RUniformAccess.h"
#include "Devices/RWindow.h"


namespace At0::Ray
{
	Mesh::Mesh() : m_GlobalUniformBufferOffset(nextOffset), m_Entity(Scene::Get().CreateEntity())
	{
		m_Entity.Emplace<Transform>();

		graphicsPipeline = Codex::Resolve<GraphicsPipeline>(Graphics::Get().GetRenderPass(),
			std::vector<std::string>{
				"Resources/Shaders/DefaultShader.vert", "Resources/Shaders/DefaultShader.frag" });

		VertexInput vertexInput(graphicsPipeline->GetVertexLayout());
		// clang-format off
		vertexInput.Emplace(Float3{ -0.5f, -0.5f, 0.0f }, Float3{ 1.0f, 0.0f, 0.0f });
		vertexInput.Emplace(Float3{  0.5f, -0.5f, 0.0f }, Float3{ 0.0f, 1.0f, 0.0f });
		vertexInput.Emplace(Float3{  0.0f,  0.5f, 0.0f }, Float3{ 0.0f, 0.0f, 1.0f });
		// clang-format on

		vertexBuffer = Codex::Resolve<VertexBuffer>("Triangle", vertexInput);

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2 };
		indexBuffer = Codex::Resolve<IndexBuffer>("012", indices);

		uniformAccess = MakeScope<UniformAccess>(*graphicsPipeline);


		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = graphicsPipeline->GetDescriptorPool();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &graphicsPipeline->GetDescriptorSetLayout();

		RAY_VK_THROW_FAILED(
			vkAllocateDescriptorSets(Graphics::Get().GetDevice(), &allocInfo, &descSet),
			"Failed to allocate descriptor set.");

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = UniformBufferSynchronizer::Get().GetBuffer();
		bufferInfo.offset = m_GlobalUniformBufferOffset;
		bufferInfo.range = sizeof(glm::mat4) * 3;

		uint32_t minBufferAlignment = Graphics::Get()
										  .GetPhysicalDevice()
										  .GetProperties()
										  .limits.minUniformBufferOffsetAlignment;

		nextOffset += bufferInfo.range < minBufferAlignment && bufferInfo.range != 0 ?
						  minBufferAlignment :
						  bufferInfo.range;

		VkWriteDescriptorSet descWrite{};
		descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWrite.dstSet = descSet;
		descWrite.dstBinding = 0;
		descWrite.dstArrayElement = 0;
		descWrite.descriptorCount = 1;
		descWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descWrite.pImageInfo = nullptr;
		descWrite.pBufferInfo = &bufferInfo;
		descWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(Graphics::Get().GetDevice(), 1, &descWrite, 0, nullptr);
	}

	Mesh::~Mesh() {}

	void Mesh::Update()
	{
		uniformAccess->Resolve<Shader::Stage::Vertex>("Transforms", "model")
			.Update(m_Entity.Get<Transform>().ToMatrix(), m_GlobalUniformBufferOffset);
		uniformAccess->Resolve<Shader::Stage::Vertex>("Transforms", "view")
			.Update(Graphics::Get().cam.Matrices.View, m_GlobalUniformBufferOffset);
		uniformAccess->Resolve<Shader::Stage::Vertex>("Transforms", "proj")
			.Update(Graphics::Get().cam.Matrices.Perspective, m_GlobalUniformBufferOffset);
	}

	void Mesh::CmdBind(const CommandBuffer& cmdBuff)
	{
		VkDeviceSize offsets[] = { 0 };
		VkBuffer vbuff = *vertexBuffer;
		VkBuffer idxBuff = *indexBuffer;
		vkCmdBindDescriptorSets(cmdBuff, VK_PIPELINE_BIND_POINT_GRAPHICS,
			graphicsPipeline->GetLayout(), 0, 1, &descSet, 0, nullptr);
		vkCmdBindVertexBuffers(cmdBuff, 0, 1, &vbuff, offsets);
		vkCmdBindIndexBuffer(cmdBuff, idxBuff, 0, VK_INDEX_TYPE_UINT16);
	}

	void Mesh::CmdDraw(const CommandBuffer& cmdBuff)
	{
		vkCmdDrawIndexed(cmdBuff, indexBuffer->GetNumberOfIndices(), 1, 0, 0, 0);
	}
}  // namespace At0::Ray
