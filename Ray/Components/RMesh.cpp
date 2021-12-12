#include "RMesh.h"

#include "RMeshRenderer.h"
#include "RHierachyComponent.h"

#include "Graphics/RCodex.h"
#include "Shading/RMaterial.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"

#include "Utils/RModel.h"
#include "Utils/RGeometricPrimitives.h"


namespace At0::Ray
{
	Mesh::Mesh(Entity entity, Mesh::VertexData vertexData)
		: Component(entity), m_VertexBuffer(std::move(vertexData.vertexBuffer)),
		  m_IndexBuffer(std::move(vertexData.indexBuffer)) RAY_DEBUG_FLAG(, m_Name(vertexData.name))
	{
		if (vertexData.material)
			if (!GetEntity().Has<MeshRenderer>())
				GetEntity().Emplace<MeshRenderer>(std::move(vertexData.material));
			else
				GetEntity().Get<MeshRenderer>().SetMaterial(std::move(vertexData.material));


		if (vertexData.children.size() != 0)
		{
			if (GetEntity().Has<HierachyComponent>())
				GetEntity().Get<HierachyComponent>().SetChildren(std::move(vertexData.children));
			else
				GetEntity().Emplace<HierachyComponent>().SetChildren(
					std::move(vertexData.children));
		}


		for (Entity child : vertexData.children)
		{
			if (!child.Has<HierachyComponent>())
				child.Emplace<HierachyComponent>().SetParent(GetEntity());
			else
				child.Get<HierachyComponent>().SetParent(GetEntity());
		}
	}

	Mesh::VertexData Mesh::Triangle(Ref<Material> material)
	{
		IndexedTriangleList mesh =
			IndexedTriangleList::Triangle(material->GetGraphicsPipeline().GetShader());
		return Mesh::VertexData{ Codex::Resolve<VertexBuffer>(
									 mesh.uniqueTag, std::move(mesh.vertices)),
			Codex::Resolve<IndexBuffer>(mesh.uniqueTag, std::move(mesh.indices)), nullptr, {},
			RAY_DEBUG_FLAG(mesh.uniqueTag) };
	}

	Mesh::VertexData Mesh::Plane(Ref<Material> material)
	{
		IndexedTriangleList mesh =
			IndexedTriangleList::Plane(material->GetGraphicsPipeline().GetShader());
		return Mesh::VertexData{ Codex::Resolve<VertexBuffer>(
									 mesh.uniqueTag, std::move(mesh.vertices)),
			Codex::Resolve<IndexBuffer>(mesh.uniqueTag, std::move(mesh.indices)), nullptr, {},
			RAY_DEBUG_FLAG(mesh.uniqueTag) };
	}

	Mesh::VertexData Mesh::Import(std::string_view filepath, Ref<Material> material)
	{
		return Model::Acquire(filepath, std::move(material))->GetVertexData();
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
