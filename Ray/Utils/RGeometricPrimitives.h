#pragma once

#include "../Core/RMath.h"
#include "../Graphics/Buffers/RIndexBuffer.h"
#include "../Core/RDynamicVertex.h"


namespace At0::Ray
{
	class Shader;

	struct IndexedTriangleList
	{
		static IndexedTriangleList Triangle(const Shader& shader);
		static IndexedTriangleList Plane(const Shader& shader);
		static IndexedTriangleList HalfCircle(const Shader& shader, int segments, float radius);
		static IndexedTriangleList Circle(const Shader& shader, int segments, float radius);
		static IndexedTriangleList Cube(const Shader& shader);
		static IndexedTriangleList UVSphere(
			const Shader& shader, float radius, int latDiv, int longDiv);
		static IndexedTriangleList Cylinder(const Shader& shader, int segments, float radius);
		static IndexedTriangleList Cone(const Shader& shader, int segments, float radius);

		static IndexedTriangleList Append(
			const IndexedTriangleList& first, const IndexedTriangleList& last);

		DynamicVertex vertices;
		std::vector<IndexBuffer::Type> indices;
		std::string uniqueTag;
	};
}  // namespace At0::Ray
