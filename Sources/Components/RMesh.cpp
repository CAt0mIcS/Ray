#include "Rpch.h"
#include "RMesh.h"

#include "Graphics/Core/RCodex.h"
#include "Graphics/Buffers/RVertexBuffer.h"
#include "Graphics/Buffers/RIndexBuffer.h"

#include "Core/RVertex.h"


namespace At0::Ray
{
	Mesh Mesh::Triangle(Material material)
	{
		VertexInput vertexInput(material.GetVertexLayout());
		vertexInput.Emplace(Float3{ -0.5f, -0.5f, 0.0f });
		vertexInput.Emplace(Float3{ 0.5f, -0.5f, 0.0f });
		vertexInput.Emplace(Float3{ 0.0f, 0.5f, 0.0f });

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2 };

		Ref<VertexBuffer> vertexBuffer = Codex::Resolve<VertexBuffer>("Triangle", vertexInput);
		Ref<IndexBuffer> indexBuffer = Codex::Resolve<IndexBuffer>("012", indices);

		return { std::move(vertexBuffer), std::move(indexBuffer), std::move(material) };
	}

	Mesh::Mesh(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Material material)
		: m_VertexBuffer(std::move(vertexBuffer)), m_IndexBuffer(std::move(indexBuffer)),
		  m_Material(std::move(material))
	{
	}
}  // namespace At0::Ray