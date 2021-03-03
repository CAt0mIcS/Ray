#pragma once

#include "../../Graphics/RVertex.h"
#include "../../Graphics/Buffers/RIndexBuffer.h"


namespace At0::Ray
{
	struct IndexedTriangleList
	{
		static IndexedTriangleList Triangle(const VertexLayout& layout);
		static IndexedTriangleList Plane(const VertexLayout& layout);
		static IndexedTriangleList Circle(
			const VertexLayout& layout, int segments, float radius = 1.0f);

		static IndexedTriangleList Cube(const VertexLayout& layout);
		static IndexedTriangleList IcoSphere(const VertexLayout& layout);
		static IndexedTriangleList UVSphere(const VertexLayout& layout);

		VertexInput vertices;
		std::vector<IndexBuffer::Type> indices;
		std::string vertexTag;
		std::string indexTag;
	};
}  // namespace At0::Ray
