#include "Rpch.h"
#include "RMesh.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Core/RLogicalDevice.h"

#include "Core/RVertex.h"
#include "Utils/RException.h"

#include "Graphics/Pipelines/RUniformAccess.h"
#include "Devices/RWindow.h"


namespace At0::Ray
{
	Mesh::Mesh()
	{
		graphicsPipeline = MakeScope<GraphicsPipeline>(Graphics::Get().GetRenderPass(),
			std::vector<std::string>{
				"Resources/Shaders/DefaultShader.vert", "Resources/Shaders/DefaultShader.frag" });

		VertexInput vertexInput(graphicsPipeline->GetVertexLayout());
		// clang-format off
		vertexInput.Emplace(Float3{ -0.5f, -0.5f, 0.0f }, Float3{ 1.0f, 0.0f, 0.0f });
		vertexInput.Emplace(Float3{  0.5f, -0.5f, 0.0f }, Float3{ 0.0f, 1.0f, 0.0f });
		vertexInput.Emplace(Float3{  0.0f,  0.5f, 0.0f }, Float3{ 0.0f, 0.0f, 1.0f });
		// clang-format on

		vertexBuffer = MakeScope<VertexBuffer>(vertexInput);

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2 };
		indexBuffer = MakeScope<IndexBuffer>(indices);
		uniformAccess = MakeScope<UniformAccess>(*graphicsPipeline);

		UInt2 size = Window::Get().GetSize();

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
		bufferInfo.offset =
			uniformAccess->Resolve(Shader::Stage::Vertex, "modelViewProj", "Transforms")
				.GetOffset();
		bufferInfo.range = sizeof(Matrix) * 3;

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
		glm::mat4 model = glm::identity<glm::mat4>();
		// glm::mat4 view = glm::lookAt(glm::vec3{ 2.0f, 2.0f, 2.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f
		// }, glm::vec3{ 0.0f, 0.0f, 1.0f });
		// glm::mat4 proj = glm::perspective(45.0f, (float)size.x / (float)size.y, 0.1f, 256.0f);

		glm::mat4 view = Graphics::Get().cam.Matrices.View;
		glm::mat4 proj = Graphics::Get().cam.Matrices.Perspective;

		uniformAccess->Resolve(Shader::Stage::Vertex, "modelViewProj", "Transforms") =
			proj * view * model;
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
