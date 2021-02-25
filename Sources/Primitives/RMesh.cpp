#include "Rpch.h"
#include "RMesh.h"

#include <numeric>

#include "Graphics/RGraphics.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/RDescriptor.h"
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
		camUniformBuffer = MakeScope<UniformBuffer>(128);

		descSet = MakeScope<DescriptorSet>(*graphicsPipeline, 0);
		camDescSet = MakeScope<DescriptorSet>(*graphicsPipeline, 1);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = BufferSynchronizer::Get().GetUniformBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(Matrix);

		VkDescriptorBufferInfo bufferInfoCam{};
		bufferInfoCam.buffer = *camUniformBuffer;
		bufferInfoCam.offset = 0;
		bufferInfoCam.range = sizeof(Matrix) * 2;


		VkWriteDescriptorSet descWrite{};
		descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWrite.dstSet = *descSet;
		descWrite.dstBinding = 0;
		descWrite.dstArrayElement = 0;
		descWrite.descriptorCount = 1;
		descWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descWrite.pImageInfo = nullptr;
		descWrite.pBufferInfo = &bufferInfo;
		descWrite.pTexelBufferView = nullptr;

		VkWriteDescriptorSet descWriteCam{};
		descWriteCam.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWriteCam.dstSet = *camDescSet;
		descWriteCam.dstBinding = 1;
		descWriteCam.dstArrayElement = 0;
		descWriteCam.descriptorCount = 1;
		descWriteCam.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descWriteCam.pImageInfo = nullptr;
		descWriteCam.pBufferInfo = &bufferInfoCam;
		descWriteCam.pTexelBufferView = nullptr;

		DescriptorSet::Update({ descWrite, descWriteCam });
	}

	Mesh::~Mesh() {}

	void Mesh::Update()
	{
		uniformAccess->Resolve<Shader::Stage::Vertex>("Transforms", "model")
			.Update(m_Entity.Get<Transform>().ToMatrix(), 0);
		// uniformAccess->Resolve<Shader::Stage::Vertex>("Transforms", "view")
		//	.Update(Graphics::Get().cam.Matrices.View, m_GlobalUniformBufferOffset);
		// uniformAccess->Resolve<Shader::Stage::Vertex>("Transforms", "proj")
		//	.Update(Graphics::Get().cam.Matrices.Perspective, m_GlobalUniformBufferOffset);

		camUniformBuffer->Update(&Camera::Get().Matrices.View, sizeof(Matrix), 0);
		camUniformBuffer->Update(
			&Camera::Get().Matrices.Perspective, sizeof(Matrix), sizeof(Matrix));
	}

	void Mesh::CmdBind(const CommandBuffer& cmdBuff)
	{
		graphicsPipeline->CmdBind(cmdBuff);

		VkDescriptorSet descSets[] = { *descSet, *camDescSet };


		VkDescriptorSet dscSet = *descSet;
		VkDescriptorSet camDscSet = *camDescSet;
		vkCmdBindDescriptorSets(cmdBuff, graphicsPipeline->GetBindPoint(),
			graphicsPipeline->GetLayout(),
			0,	// bind to binding point 0 (DescriptorSetLayout in pipeline)
			1, &dscSet, 0, nullptr);
		vkCmdBindDescriptorSets(cmdBuff, graphicsPipeline->GetBindPoint(),
			graphicsPipeline->GetLayout(),
			1,	// bind to binding point 1 (DescriptorSetLayout in pipeline)
			1, &camDscSet, 0, nullptr);

		// vkCmdBindDescriptorSets(cmdBuff, graphicsPipeline->GetBindPoint(),
		//	graphicsPipeline->GetLayout(), 0, std::size(descSets), descSets, 0, nullptr);

		// descSet->CmdBind(cmdBuff);
		// camDescSet->CmdBind(cmdBuff);

		vertexBuffer->CmdBind(cmdBuff);
		indexBuffer->CmdBind(cmdBuff);
	}

	void Mesh::CmdDraw(const CommandBuffer& cmdBuff)
	{
		vkCmdDrawIndexed(cmdBuff, indexBuffer->GetNumberOfIndices(), 1, 0, 0, 0);
	}
}  // namespace At0::Ray
