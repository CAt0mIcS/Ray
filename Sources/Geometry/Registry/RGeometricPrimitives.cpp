#include "Rpch.h"
#include "../../Core/RMath.h"
#include "RGeometricPrimitives.h"

#include "Utils/RString.h"


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

	//	std::string tag = String::Serialize("Circle-segs{0}-radius{1}", segments, radius);
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

	IndexedTriangleList IndexedTriangleList::UVSphere(
		const VertexLayout& layout, float radius, int latDiv, int longDiv)
	{
		const Float4 base = Float4{ 0.0f, 0.0f, radius, 0.0f };
		const float lattitudeAngle = Math::PI<float> / latDiv;
		const float longitudeAngle = 2.0f * Math::PI<float> / longDiv;

		VertexInput vb{ std::move(layout) };
		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			Float4 latBase =
				base * glm::rotate(Matrix(1.0f), lattitudeAngle * iLat, Float3(1.0f, 0.0f, 0.0f));

			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				Float3 calculatedPos = latBase * glm::rotate(Matrix(1.0f), longitudeAngle * iLong,
													 Float3(0.0f, 0.0f, 1.0f));

				vb.Emplace(calculatedPos);
			}
		}

		// add the cap vertices
		const uint16_t iNorthPole = (uint16_t)vb.Size();
		{
			Float3 northPos = base;
			vb.Emplace(northPos);
		}
		const uint16_t iSouthPole = (uint16_t)vb.Size();
		{
			Float3 southPos = -base;
			vb.Emplace(southPos);
		}

		const auto calcIdx = [latDiv, longDiv](IndexBuffer::Type iLat, IndexBuffer::Type iLong) {
			return iLat * longDiv + iLong;
		};

		std::vector<IndexBuffer::Type> indices;
		for (IndexBuffer::Type iLat = 0; iLat < latDiv - 2; iLat++)
		{
			for (IndexBuffer::Type iLong = 0; iLong < longDiv - 1; iLong++)
			{
				indices.push_back(calcIdx(iLat + 1, iLong + 1));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat, iLong));
			}
			// wrap band
			indices.push_back(calcIdx(iLat + 1, 0));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat, longDiv - 1));
		}

		// cap fans
		for (IndexBuffer::Type iLong = 0; iLong < longDiv - 1; iLong++)
		{
			// north
			indices.push_back(calcIdx(0, iLong + 1));
			indices.push_back(calcIdx(0, iLong));
			indices.push_back(iNorthPole);
			// south
			indices.push_back(iSouthPole);
			indices.push_back(calcIdx(latDiv - 2, iLong));
			indices.push_back(calcIdx(latDiv - 2, iLong + 1));
		}
		// wrap triangles
		// north
		indices.push_back(calcIdx(0, 0));
		indices.push_back(calcIdx(0, longDiv - 1));
		indices.push_back(iNorthPole);
		// south
		indices.push_back(iSouthPole);
		indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.push_back(calcIdx(latDiv - 2, 0));

		std::ostringstream oss;
		oss << "UVSPhere#" << radius << "#" << latDiv << "#" << longDiv;

		return { vb, indices, oss.str(), oss.str() };
	}

	// IndexedTriangleList IndexedTriangleList::IcoSphere(const VertexLayout& layout) {}
}  // namespace At0::Ray
