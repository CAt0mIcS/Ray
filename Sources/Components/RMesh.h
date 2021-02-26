#pragma once

#include "../RBase.h"
#include "RMaterial.h"


namespace At0::Ray
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexInput;

	class RAY_EXPORT Mesh
	{
	public:
		static Mesh Triangle(Material material);

	private:
		Mesh(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Material material);

	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		Material m_Material;
	};
}  // namespace At0::Ray
