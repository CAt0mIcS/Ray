#pragma once

#include "../../Graphics/RVertex.h"
#include "../../Core/RMath.h"
#include "../../Graphics/Buffers/RIndexBuffer.h"


namespace At0::Ray
{
	struct IndexedTriangleList
	{
		static IndexedTriangleList Triangle(const VertexLayout& layout);
		static IndexedTriangleList Plane(const VertexLayout& layout);
		static IndexedTriangleList HalfCircle(
			const VertexLayout& layout, int segments = 360, float radius = 1.0f);
		static IndexedTriangleList Circle(
			const VertexLayout& layout, int segments = 360, float radius = 1.0f);

		static IndexedTriangleList Cube(const VertexLayout& layout);
		static IndexedTriangleList IcoSphere(const VertexLayout& layout);
		static IndexedTriangleList UVSphere(
			const VertexLayout& layout, float radius, int latDiv = 12, int longDiv = 24);
		static IndexedTriangleList Cylinder(
			const VertexLayout& layout, int segments = 360, float radius = 1.0f);
		static IndexedTriangleList Vector(const VertexLayout& layout, const Float3& headPos);

		VertexInput vertices;
		std::vector<IndexBuffer::Type> indices;
		std::string tag;

	private:
		static IndexedTriangleList Append(
			const IndexedTriangleList& l1, const IndexedTriangleList& l2);
	};
}  // namespace At0::Ray
