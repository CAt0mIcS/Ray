#include "RMesh.h"

#include "RMeshRenderingResources.h"
#include "RHierachyComponent.h"

#include "Graphics/RCodex.h"
#include "Graphics/Pipelines/Shader/RShader.h"
#include "Graphics/Core/RRenderContext.h"
#include "Shading/RMaterial.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Buffers/RIndexBuffer.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/Pipelines/RGraphicsPipeline.h"

#include "Utils/RGeometricPrimitives.h"


namespace At0::Ray
{
	Mesh::Mesh(Entity entity, Mesh::Data vertexData)
		: Component(entity), m_VertexBuffer(std::move(vertexData.vertexBuffer)),
		  m_IndexBuffer(std::move(vertexData.indexBuffer)) RAY_DEBUG_FLAG(, m_Name(vertexData.name))
	{
		if (vertexData.material)
		{
			if (!GetEntity().Has<MeshRenderingResources>())
				GetEntity().Emplace<MeshRenderingResources>(std::move(vertexData.material));
			else
				GetEntity().Get<MeshRenderingResources>().SetMaterial(
					std::move(vertexData.material));
		}


		// if (vertexData.children.size() != 0)
		//{
		//	if (GetEntity().Has<HierachyComponent>())
		//		GetEntity().Get<HierachyComponent>().SetChildren(std::move(vertexData.children));
		//	else
		//		GetEntity().Emplace<HierachyComponent>().SetChildren(
		//			std::move(vertexData.children));
		//}


		// for (Entity child : vertexData.children)
		//{
		//	if (!child.Has<HierachyComponent>())
		//		child.Emplace<HierachyComponent>().SetParent(GetEntity());
		//	else
		//		child.Get<HierachyComponent>().SetParent(GetEntity());
		//}
	}

	Mesh::Data Mesh::Triangle(Ref<Material> material)
	{
		auto& shader = material->GetGraphicsPipeline().GetShader();
		auto transientPool = shader.GetRenderContext().graphics.GetTransientCommandPool();

		IndexedTriangleList mesh = IndexedTriangleList::Triangle(shader);
		return Mesh::Data{ Codex::Resolve<VertexBuffer>(shader.GetRenderContext(), *transientPool,
							   mesh.uniqueTag, std::move(mesh.vertices)),
			Codex::Resolve<IndexBuffer>(
				shader.GetRenderContext(), *transientPool, mesh.uniqueTag, std::move(mesh.indices)),
			std::move(material), RAY_DEBUG_FLAG(mesh.uniqueTag) };
	}

	Mesh::Data Mesh::Plane(Ref<Material> material)
	{
		auto& shader = material->GetGraphicsPipeline().GetShader();
		auto transientPool = shader.GetRenderContext().graphics.GetTransientCommandPool();

		IndexedTriangleList mesh = IndexedTriangleList::Plane(shader);
		return Mesh::Data{ Codex::Resolve<VertexBuffer>(shader.GetRenderContext(), *transientPool,
							   mesh.uniqueTag, std::move(mesh.vertices)),
			Codex::Resolve<IndexBuffer>(
				shader.GetRenderContext(), *transientPool, mesh.uniqueTag, std::move(mesh.indices)),
			std::move(material), RAY_DEBUG_FLAG(mesh.uniqueTag) };
	}

	Mesh::Data Mesh::HalfCircle(Ref<Material> material, int segments, float radius)
	{
		auto& shader = material->GetGraphicsPipeline().GetShader();
		auto transientPool = shader.GetRenderContext().graphics.GetTransientCommandPool();

		IndexedTriangleList mesh = IndexedTriangleList::HalfCircle(shader, segments, radius);
		return Mesh::Data{ Codex::Resolve<VertexBuffer>(shader.GetRenderContext(), *transientPool,
							   mesh.uniqueTag, std::move(mesh.vertices)),
			Codex::Resolve<IndexBuffer>(
				shader.GetRenderContext(), *transientPool, mesh.uniqueTag, std::move(mesh.indices)),
			std::move(material), RAY_DEBUG_FLAG(mesh.uniqueTag) };
	}

	Mesh::Data Mesh::Circle(Ref<Material> material, int segments, float radius)
	{
		auto& shader = material->GetGraphicsPipeline().GetShader();
		auto transientPool = shader.GetRenderContext().graphics.GetTransientCommandPool();

		IndexedTriangleList mesh = IndexedTriangleList::Circle(shader, segments, radius);
		return Mesh::Data{ Codex::Resolve<VertexBuffer>(shader.GetRenderContext(), *transientPool,
							   mesh.uniqueTag, std::move(mesh.vertices)),
			Codex::Resolve<IndexBuffer>(
				shader.GetRenderContext(), *transientPool, mesh.uniqueTag, std::move(mesh.indices)),
			std::move(material), RAY_DEBUG_FLAG(mesh.uniqueTag) };
	}

	Mesh::Data Mesh::Cube(Ref<Material> material)
	{
		auto& shader = material->GetGraphicsPipeline().GetShader();
		auto transientPool = shader.GetRenderContext().graphics.GetTransientCommandPool();

		IndexedTriangleList mesh = IndexedTriangleList::Cube(shader);
		return Mesh::Data{ Codex::Resolve<VertexBuffer>(shader.GetRenderContext(), *transientPool,
							   mesh.uniqueTag, std::move(mesh.vertices)),
			Codex::Resolve<IndexBuffer>(
				shader.GetRenderContext(), *transientPool, mesh.uniqueTag, std::move(mesh.indices)),
			std::move(material), RAY_DEBUG_FLAG(mesh.uniqueTag) };
	}

	Mesh::Data Mesh::UVSphere(Ref<Material> material, float radius, int latDiv, int longDiv)
	{
		auto& shader = material->GetGraphicsPipeline().GetShader();
		auto transientPool = shader.GetRenderContext().graphics.GetTransientCommandPool();

		IndexedTriangleList mesh = IndexedTriangleList::UVSphere(shader, radius, latDiv, longDiv);
		return Mesh::Data{ Codex::Resolve<VertexBuffer>(shader.GetRenderContext(), *transientPool,
							   mesh.uniqueTag, std::move(mesh.vertices)),
			Codex::Resolve<IndexBuffer>(
				shader.GetRenderContext(), *transientPool, mesh.uniqueTag, std::move(mesh.indices)),
			std::move(material), RAY_DEBUG_FLAG(mesh.uniqueTag) };
	}

	Mesh::Data Mesh::Cylinder(Ref<Material> material, int segments, float radius)
	{
		auto& shader = material->GetGraphicsPipeline().GetShader();
		auto transientPool = shader.GetRenderContext().graphics.GetTransientCommandPool();

		IndexedTriangleList mesh = IndexedTriangleList::Cylinder(shader, segments, radius);
		return Mesh::Data{ Codex::Resolve<VertexBuffer>(shader.GetRenderContext(), *transientPool,
							   mesh.uniqueTag, std::move(mesh.vertices)),
			Codex::Resolve<IndexBuffer>(
				shader.GetRenderContext(), *transientPool, mesh.uniqueTag, std::move(mesh.indices)),
			std::move(material), RAY_DEBUG_FLAG(mesh.uniqueTag) };
	}

	Mesh::Data Mesh::Cone(Ref<Material> material, int segments, float radius)
	{
		auto& shader = material->GetGraphicsPipeline().GetShader();
		auto transientPool = shader.GetRenderContext().graphics.GetTransientCommandPool();

		IndexedTriangleList mesh = IndexedTriangleList::Cone(shader, segments, radius);
		return Mesh::Data{ Codex::Resolve<VertexBuffer>(shader.GetRenderContext(), *transientPool,
							   mesh.uniqueTag, std::move(mesh.vertices)),
			Codex::Resolve<IndexBuffer>(
				shader.GetRenderContext(), *transientPool, mesh.uniqueTag, std::move(mesh.indices)),
			std::move(material), RAY_DEBUG_FLAG(mesh.uniqueTag) };
	}

	Mesh::Data Mesh::Vector(Ref<Material> material)
	{
		auto& shader = material->GetGraphicsPipeline().GetShader();
		auto transientPool = shader.GetRenderContext().graphics.GetTransientCommandPool();

		IndexedTriangleList mesh = IndexedTriangleList::Vector(shader);
		return Mesh::Data{ Codex::Resolve<VertexBuffer>(shader.GetRenderContext(), *transientPool,
							   mesh.uniqueTag, std::move(mesh.vertices)),
			Codex::Resolve<IndexBuffer>(
				shader.GetRenderContext(), *transientPool, mesh.uniqueTag, std::move(mesh.indices)),
			std::move(material), RAY_DEBUG_FLAG(mesh.uniqueTag) };
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
		RAY_DEBUG_FLAG(m_Name = std::move(other.m_Name));
		return *this;
	}

	Mesh::Mesh(Mesh&& other) noexcept : Component(other.m_Entity)
	{
		*this = std::move(other);
	}
}  // namespace At0::Ray
