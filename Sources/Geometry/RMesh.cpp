﻿#include "Rpch.h"
#include "RMesh.h"

#include "Graphics/RGraphics.h"
#include "Graphics/RCodex.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"
#include "Graphics/Pipelines/RUniformAccess.h"
#include "Graphics/Buffers/RBufferSynchronizer.h"
#include "Graphics/Core/RPhysicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Images/RTexture2D.h"

#include "Registry/RGeometricPrimitives.h"
#include "Scene/REntity.h"
#include "Graphics/RVertex.h"


namespace At0::Ray
{
	Mesh::Mesh(Entity& entity, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer,
		Material material)
		: Component(entity), m_VertexBuffer(std::move(vertexBuffer)),
		  m_IndexBuffer(std::move(indexBuffer)), m_Material(std::move(material)),
		  m_DescriptorSet(m_Material.GetGraphicsPipeline().GetDescriptorPool(),
			  m_Material.GetGraphicsPipeline()
				  .GetDescriptorSetLayouts()[1 /*Descriptor set (set = 1)*/],
			  m_Material.GetGraphicsPipeline().GetBindPoint(),
			  m_Material.GetGraphicsPipeline().GetLayout(), 1)
	{
		Setup();
	}

	Mesh::Mesh(Entity& entity, MeshData data)
		: Component(entity), m_VertexBuffer(std::move(data.vertexBuffer)),
		  m_IndexBuffer(std::move(data.indexBuffer)), m_Material(std::move(data.material)),
		  m_DescriptorSet(m_Material.GetGraphicsPipeline().GetDescriptorPool(),
			  m_Material.GetGraphicsPipeline().GetDescriptorSetLayouts()[1],
			  m_Material.GetGraphicsPipeline().GetBindPoint(),
			  m_Material.GetGraphicsPipeline().GetLayout(), 1)
	{
		Setup();
	}

	Mesh::MeshData Mesh::Triangle(Material material)
	{
		IndexedTriangleList triangle = IndexedTriangleList::Triangle(material.GetVertexLayout());

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(triangle.vertexTag, triangle.vertices);
		Ref<IndexBuffer> indexBuffer =
			Codex::Resolve<IndexBuffer>(triangle.indexTag, triangle.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	Mesh::MeshData Mesh::Plane(Material material)
	{
		IndexedTriangleList plane = IndexedTriangleList::Plane(material.GetVertexLayout());

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(plane.vertexTag, plane.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(plane.indexTag, plane.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	// Mesh::MeshData Mesh::Circle(Material material, int segments, float radius)
	//{
	// material.Create();
	//	IndexedTriangleList circle =
	//		IndexedTriangleList::Circle(material.GetVertexLayout(), segments, radius);

	//	Ref<VertexBuffer> vertexBuffer =
	//		Codex::Resolve<VertexBuffer>(circle.vertexTag, circle.vertices);
	//	Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(circle.indexTag, circle.indices);

	//	return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	//}

	Mesh::MeshData Mesh::Cube(Material material)
	{
		IndexedTriangleList cube = IndexedTriangleList::Cube(material.GetVertexLayout());

		Ref<VertexBuffer> vertexBuffer =
			Codex::Resolve<VertexBuffer>(cube.vertexTag, cube.vertices);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>(cube.indexTag, cube.indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	// Mesh::MeshData Mesh::IcoSphere(Material material) { return Mesh(); }

	// Mesh::MeshData Mesh::UVSphere(Material material) { return Mesh(); }

	void Mesh::Update(Delta ts, const Transform& parentTransform)
	{
		m_Uniforms.Resolve<Shader::Stage::Vertex>("PerObjectData", "model") =
			(m_Transform + parentTransform).AsMatrix();
	}

	void Mesh::Bind(const CommandBuffer& cmdBuff) const
	{
		m_Material.GetGraphicsPipeline().CmdBind(cmdBuff);
		if (m_Material.GetMaterialImage())
		{
			m_Material.GetMaterialImage()->CmdBind(cmdBuff);
			m_MaterialDescSet->CmdBind(cmdBuff);
		}

		m_DescriptorSet.CmdBind(cmdBuff);
		m_VertexBuffer->CmdBind(cmdBuff);
		m_IndexBuffer->CmdBind(cmdBuff);
	}

	void Mesh::Render(const CommandBuffer& cmdBuff) const
	{
		vkCmdDrawIndexed(cmdBuff, m_IndexBuffer->GetNumberOfIndices(), 1, 0, 0, 0);
	}

	Mesh::~Mesh() {}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		m_VertexBuffer = std::move(other.m_VertexBuffer);
		m_IndexBuffer = std::move(other.m_IndexBuffer);

		m_Material = std::move(other.m_Material);
		m_Uniforms = std::move(other.m_Uniforms);
		m_DescriptorSet = std::move(other.m_DescriptorSet);

		m_Transform = std::move(other.m_Transform);
		m_MaterialDescSet = std::move(other.m_MaterialDescSet);
		return *this;
	}

	Mesh::Mesh(Mesh&& other) noexcept
		: Component(*other.m_Entity), m_VertexBuffer(std::move(other.m_VertexBuffer)),
		  m_IndexBuffer(std::move(other.m_IndexBuffer)), m_Material(std::move(other.m_Material)),
		  m_Uniforms(std::move(other.m_Uniforms)),
		  m_DescriptorSet(std::move(other.m_DescriptorSet)),
		  m_Transform(std::move(other.m_Transform)),
		  m_MaterialDescSet(std::move(other.m_MaterialDescSet))
	{
	}

	void Mesh::Setup()
	{
		if (m_Material.GetMaterialImage())
		{
			m_MaterialDescSet =
				MakeScope<DescriptorSet>(m_Material.GetGraphicsPipeline().GetDescriptorPool(),
					m_Material.GetGraphicsPipeline()
						.GetDescriptorSetLayouts()[2 /*Descriptor set (set = 2)*/],
					m_Material.GetGraphicsPipeline().GetBindPoint(),
					m_Material.GetGraphicsPipeline().GetLayout(), 2);
		}


		// Allocate uniform buffer memory
		uint32_t alignment = Graphics::Get()
								 .GetPhysicalDevice()
								 .GetProperties()
								 .limits.minUniformBufferOffsetAlignment;
		uint32_t globalUniformBufferOffset = 0;
		BufferSynchronizer::Get().Emplace(sizeof(Matrix), alignment, &globalUniformBufferOffset);

		m_Uniforms = UniformAccess(m_Material.GetGraphicsPipeline(), globalUniformBufferOffset);

		// Update descriptor set
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer =
			BufferSynchronizer::Get().GetUniformBuffer().GetBuffer(globalUniformBufferOffset);
		bufferInfo.offset =
			BufferSynchronizer::Get().GetUniformBuffer().GetOffset(globalUniformBufferOffset);
		bufferInfo.range = sizeof(Matrix);

		if (m_Material.GetMaterialImage())
		{
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.sampler = m_Material.GetMaterialImage()->GetSampler();
			imageInfo.imageView = m_Material.GetMaterialImage()->GetImage().GetImageView();

			VkWriteDescriptorSet descWriteImg{};
			descWriteImg.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descWriteImg.dstSet = *m_MaterialDescSet;
			descWriteImg.dstBinding = 2;
			descWriteImg.dstArrayElement = 0;
			descWriteImg.descriptorCount = 1;
			descWriteImg.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descWriteImg.pImageInfo = &imageInfo;
			DescriptorSet::Update({ descWriteImg });
		}

		VkWriteDescriptorSet descWriteBuff{};
		descWriteBuff.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descWriteBuff.dstSet = m_DescriptorSet;
		descWriteBuff.dstBinding = 1;
		descWriteBuff.dstArrayElement = 0;
		descWriteBuff.descriptorCount = 1;
		descWriteBuff.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descWriteBuff.pBufferInfo = &bufferInfo;

		DescriptorSet::Update({ descWriteBuff });
	}
}  // namespace At0::Ray