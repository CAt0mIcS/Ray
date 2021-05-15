#include "Rpch.h"
#include "RMesh.h"

#include "RParentEntity.h"
#include "RMeshRenderer.h"

#include "Graphics/RCodex.h"
#include "Shading/RMaterial.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"

#include "Core/RDynamicVertex.h"
#include "Utils/RString.h"
#include "Utils/RModel.h"


namespace At0::Ray
{
	Mesh::Mesh(Entity entity, Mesh::VertexData vertexData)
		: Component(entity), m_VertexBuffer(std::move(vertexData.vertexBuffer)),
		  m_IndexBuffer(std::move(vertexData.indexBuffer))
	{
		for (Entity e : vertexData.children)
		{
			e.Emplace<ParentEntity>(entity);
		}

		// if (!vertexData.children.empty())
		//	EmplaceChildren(std::move(vertexData.children),
		//		GetEntity().Get<MeshRenderer>().GetSharedMaterial());
	}

	Mesh::VertexData Mesh::Triangle(Ref<Material> material)
	{
		DynamicVertex vertex(material->GetGraphicsPipeline().GetShader());

		bool hasPos = vertex.Has(AttributeMap<AttributeType::Position>::Semantic);
		bool hasUV = vertex.Has(AttributeMap<AttributeType::UV>::Semantic);
		bool hasNormal = vertex.Has(AttributeMap<AttributeType::Normal>::Semantic);
		bool hasTangent = vertex.Has(AttributeMap<AttributeType::Tangent>::Semantic);

		RAY_MEXPECTS(!hasNormal && !hasTangent && !hasUV,
			"[IndexedTriangleList] Normals, tangents and "
			"UV-Coordinates not supported for plane yet");

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, -0.5f, 0.0f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, -0.5f, 0.0f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.0f, 0.5f, 0.0f };

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2 };
		std::string tag = String::Serialize(
			"Triangle#012230#{0}#{1}#{2}#{3}", hasPos, hasUV, hasNormal, hasTangent);

		return Mesh::VertexData{ Codex::Resolve<VertexBuffer>(tag, vertex),
			Codex::Resolve<IndexBuffer>(tag, indices) };
	}

	Mesh::VertexData Mesh::Plane(Ref<Material> material)
	{
		DynamicVertex vertex(material->GetGraphicsPipeline().GetShader());

		bool hasPos = vertex.Has(AttributeMap<AttributeType::Position>::Semantic);
		bool hasUV = vertex.Has(AttributeMap<AttributeType::UV>::Semantic);
		bool hasNormal = vertex.Has(AttributeMap<AttributeType::Normal>::Semantic);
		bool hasTangent = vertex.Has(AttributeMap<AttributeType::Tangent>::Semantic);

		RAY_MEXPECTS(!hasNormal && !hasTangent,
			"[IndexedTriangleList] Normals and tangents not supported for plane yet");

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, -0.5f, 0.0f };
		if (hasUV)
			vertex[AttributeMap<AttributeType::UV>::Semantic] = Float2{ 1.0f, 0.0f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, -0.5f, 0.0f };
		if (hasUV)
			vertex[AttributeMap<AttributeType::UV>::Semantic] = Float2{ 0.0f, 0.0f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, 0.5f, 0.0f };
		if (hasUV)
			vertex[AttributeMap<AttributeType::UV>::Semantic] = Float2{ 0.0f, 1.0f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, 0.5f, 0.0f };
		if (hasUV)
			vertex[AttributeMap<AttributeType::UV>::Semantic] = Float2{ 1.0f, 1.0f };

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2, 2, 3, 0 };
		std::string tag =
			String::Serialize("Plane#012230#{0}#{1}#{2}#{3}", hasPos, hasUV, hasNormal, hasTangent);

		return Mesh::VertexData{ Codex::Resolve<VertexBuffer>(tag, vertex),
			Codex::Resolve<IndexBuffer>(tag, indices) };
	}

	Mesh::VertexData Mesh::Import(std::string_view filepath, Ref<Material> material)
	{
		return Model{ filepath, material }.GetVertexData();
	}

	void Mesh::CmdBind(const CommandBuffer& cmdBuff) const
	{
		m_VertexBuffer->CmdBind(cmdBuff);
		m_IndexBuffer->CmdBind(cmdBuff);

		vkCmdDrawIndexed(cmdBuff, m_IndexBuffer->GetNumberOfIndices(), 1, 0, 0, 0);
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		m_VertexBuffer = std::move(other.m_VertexBuffer);
		m_IndexBuffer = std::move(other.m_IndexBuffer);
		m_Entity = std::move(other.m_Entity);
		return *this;
	}

	Mesh::Mesh(Mesh&& other) noexcept : Component(other.m_Entity) { *this = std::move(other); }
}  // namespace At0::Ray
