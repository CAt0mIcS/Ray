#include "Rpch.h"
#include "../../Core/RMath.h"
#include "RGeometricPrimitives.h"

#include "Utils/RLogger.h"


namespace At0::Ray
{
	IndexedTriangleList IndexedTriangleList::Triangle(const VertexLayout& layout)
	{
		// Compose one vertex
		VertexInput vertexInput(layout);
		vertexInput.Emplace(Float3{ -0.5f, -0.5f, 0.0f });
		vertexInput.Emplace(Float3{ 0.5f, -0.5f, 0.0f });
		vertexInput.Emplace(Float3{ 0.0f, 0.5f, 0.0f });

		// RAY_TODO: 2D objects are only visible from one side because of backface culling
		// Specifying the vertices again in the reversed order works for now
		std::vector<IndexBuffer::Type> indices{ 0, 1, 2 /*|*/, 2, 1, 0 };

		return { vertexInput, indices, "Triangle", "012" };
	}

	IndexedTriangleList IndexedTriangleList::Plane(const VertexLayout& layout)
	{
		VertexInput vertexInput(layout);
		vertexInput.Emplace(Float3{ -0.5f, -0.5f, 0.0f }, Float2{ 1.0f, 0.0f });
		vertexInput.Emplace(Float3{ 0.5f, -0.5f, 0.0f }, Float2{ 0.0f, 0.0f });
		vertexInput.Emplace(Float3{ 0.5f, 0.5f, 0.0f }, Float2{ 0.0f, 1.0f });
		vertexInput.Emplace(Float3{ -0.5f, 0.5f, 0.0f }, Float2{ 1.0f, 1.0f });

		// RAY_TODO: 2D objects are only visible from one side because of backface culling
		// Specifying the vertices again in the reversed order works for now
		std::vector<IndexBuffer::Type> indices{ 0, 1, 2, 2, 3, 0 /*|*/, 0, 3, 2, 2, 1, 0 };

		return { vertexInput, indices, "Plane", "012230032210" };
	}

	// IndexedTriangleList IndexedTriangleList::Circle(
	//	const VertexLayout& layout, int segments, float radius)
	//{
	//	VertexInput vertices(layout);
	//	std::vector<IndexBuffer::Type> indices;

	//	std::string tag = SerializeString("Circle-segs{0}-radius{1}", segments, radius);
	//	return { vertices, indices, tag, tag };
	//}

	IndexedTriangleList IndexedTriangleList::Cube(const VertexLayout& layout)
	{
		VertexInput vertexInput(layout);
		vertexInput.Emplace(Float3{ -0.5f, -0.5f, -0.5f });
		vertexInput.Emplace(Float3{ 0.5f, -0.5f, -0.5f });
		vertexInput.Emplace(Float3{ -0.5f, 0.5f, -0.5f });
		vertexInput.Emplace(Float3{ 0.5f, 0.5f, -0.5f });
		vertexInput.Emplace(Float3{ -0.5f, -0.5f, 0.5f });
		vertexInput.Emplace(Float3{ 0.5f, -0.5f, 0.5f });
		vertexInput.Emplace(Float3{ -0.5f, 0.5f, 0.5f });
		vertexInput.Emplace(Float3{ 0.5f, 0.5f, 0.5f });

		std::vector<IndexBuffer::Type> indices{ 0, 2, 1, 2, 3, 1, 1, 3, 5, 3, 7, 5, 2, 6, 3, 3, 6,
			7, 4, 5, 7, 4, 7, 6, 0, 4, 2, 2, 4, 6, 0, 1, 4, 1, 5, 4 };

		return { vertexInput, indices, "Cube", "Cube" };
	}

	// IndexedTriangleList IndexedTriangleList::IcoSphere(const VertexLayout& layout) {}

	// IndexedTriangleList IndexedTriangleList::UVSphere(const VertexLayout& layout) {}
}  // namespace At0::Ray
