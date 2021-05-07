#include "Rpch.h"
#include "../Core/RMath.h"
#include "RGeometricPrimitives.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	IndexedTriangleList IndexedTriangleList::Triangle(const VertexLayout& layout)
	{
		VertexInput vertexInput(layout);

		vertexInput.Emplace(Float3{ -0.5f, -0.5f, 0.0f });
		vertexInput.Emplace(Float3{ 0.5f, -0.5f, 0.0f });
		vertexInput.Emplace(Float3{ 0.0f, 0.5f, 0.0f });

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2 };

		return { vertexInput, indices, "Triangle#012" };
	}

	IndexedTriangleList IndexedTriangleList::Plane(const VertexLayout& layout, Vertex::Flags flags)
	{
		VertexInput vertexInput(layout);

		if (flags & Vertex::Position3D && flags & Vertex::TextureCoordinate)
		{
			vertexInput.Emplace(Float3{ -0.5f, -0.5f, 0.0f }, Float2{ 1.0f, 0.0f });
			vertexInput.Emplace(Float3{ 0.5f, -0.5f, 0.0f }, Float2{ 0.0f, 0.0f });
			vertexInput.Emplace(Float3{ 0.5f, 0.5f, 0.0f }, Float2{ 0.0f, 1.0f });
			vertexInput.Emplace(Float3{ -0.5f, 0.5f, 0.0f }, Float2{ 1.0f, 1.0f });
		}
		else if (flags & Vertex::Position3D)
		{
			vertexInput.Emplace(Float3{ -0.5f, -0.5f, 0.0f });
			vertexInput.Emplace(Float3{ 0.5f, -0.5f, 0.0f });
			vertexInput.Emplace(Float3{ 0.5f, 0.5f, 0.0f });
			vertexInput.Emplace(Float3{ -0.5f, 0.5f, 0.0f });
		}
		else
			RAY_ASSERT(false, "[IndexedTriangleList] Unsuported flags configuration.");

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2, 2, 3, 0 };

		return { vertexInput, indices, String::Serialize("Plane#012230032210#{0}", (int)flags) };
	}

	IndexedTriangleList IndexedTriangleList::HalfCircle(
		const VertexLayout& layout, int segments, float radius)
	{
		VertexInput vertexInput(layout);

		RAY_MEXPECTS(segments % 2 == 0,
			"[IndexedTriangleList] Cannot create half circle with {0} segments because this value "
			"is "
			"uneven",
			segments);

		// circle middle
		vertexInput.Emplace(Float3(0.0f, 0.0f, 0.0f));

		const Float4 base = Float4{ 0.0f, 0.0f, radius, 0.0f };
		const float lattitudeAngle = Math::PI<float> / segments;

		for (int segment = 0; segment <= segments; ++segment)
		{
			Float3 calculatedPos = base * glm::rotate(Matrix(1.0f), lattitudeAngle * segment,
											  Float3(0.0f, -1.0f, 0.0f));

			// Rotate circle to face camera instead of floor
			std::swap(calculatedPos.z, calculatedPos.x);
			std::swap(calculatedPos.z, calculatedPos.y);

			vertexInput.Emplace(calculatedPos);
		}

		std::vector<IndexBuffer::Type> indices;
		for (int segment = 0; segment <= segments; ++segment)
		{
			// circle middle
			indices.emplace_back(0);

			indices.emplace_back(segment);
			indices.emplace_back(segment + 1);
		}

		std::string tag = String::Serialize("HalfCircle#{0}#{1}", segments, radius);
		return { vertexInput, indices, tag };
	}

	IndexedTriangleList IndexedTriangleList::Circle(
		const VertexLayout& layout, int segments, float radius)
	{
		VertexInput vertexInput(layout);

		RAY_MEXPECTS(segments % 2 == 0,
			"[IndexedTriangleList] Cannot create circle with {0} segments because this value is "
			"uneven",
			segments);
		// Split segments for half circle
		segments /= 2;

		// circle middle
		vertexInput.Emplace(Float3(0.0f, 0.0f, 0.0f));

		const Float4 base = Float4{ 0.0f, 0.0f, radius, 0.0f };
		const float lattitudeAngle = Math::PI<float> / segments;

		for (int segment = 0; segment <= segments; ++segment)
		{
			Float3 calculatedPos = base * glm::rotate(Matrix(1.0f), lattitudeAngle * segment,
											  Float3(0.0f, -1.0f, 0.0f));

			// Rotate circle to face camera instead of floor
			std::swap(calculatedPos.z, calculatedPos.y);

			vertexInput.Emplace(calculatedPos);
			vertexInput.Emplace(-calculatedPos);
		}

		std::vector<IndexBuffer::Type> indices;
		for (uint32_t segment = 1; segment <= segments * 2; ++segment)
		{
			indices.emplace_back(0);
			indices.emplace_back(segment);
			indices.emplace_back(segment + 2);
		}

		std::string tag = String::Serialize("Circle#{0}#{1}", segments, radius);
		return { vertexInput, indices, tag };
	}

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

		return { vertexInput, indices, "Cube#021231135375263367457476042246014154" };
	}

	IndexedTriangleList IndexedTriangleList::UVSphere(
		const VertexLayout& layout, float radius, int latDiv, int longDiv)
	{
		const Float4 base = Float4{ 0.0f, 0.0f, radius, 0.0f };
		const float lattitudeAngle = Math::PI<float> / latDiv;
		const float longitudeAngle = 2.0f * Math::PI<float> / longDiv;

		VertexInput vertexInput(layout);
		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			Float4 latBase =
				base * glm::rotate(Matrix(1.0f), lattitudeAngle * iLat, Float3(1.0f, 0.0f, 0.0f));

			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				Float3 calculatedPos = latBase * glm::rotate(Matrix(1.0f), longitudeAngle * iLong,
													 Float3(0.0f, 0.0f, 1.0f));

				vertexInput.Emplace(calculatedPos);
			}
		}

		// add the cap vertices
		const uint16_t iNorthPole = (uint16_t)vertexInput.Size();
		{
			Float3 northPos = base;
			vertexInput.Emplace(northPos);
		}
		const uint16_t iSouthPole = (uint16_t)vertexInput.Size();
		{
			Float3 southPos = -base;
			vertexInput.Emplace(southPos);
		}

		const auto calcIdx = [latDiv, longDiv](IndexBuffer::Type iLat, IndexBuffer::Type iLong) {
			return iLat * longDiv + iLong;
		};

		std::vector<IndexBuffer::Type> indices;
		for (IndexBuffer::Type iLat = 0; iLat < latDiv - 2; iLat++)
		{
			for (IndexBuffer::Type iLong = 0; iLong < longDiv - 1; iLong++)
			{
				indices.emplace_back(calcIdx(iLat + 1, iLong + 1));
				indices.emplace_back(calcIdx(iLat + 1, iLong));
				indices.emplace_back(calcIdx(iLat, iLong + 1));
				indices.emplace_back(calcIdx(iLat, iLong + 1));
				indices.emplace_back(calcIdx(iLat + 1, iLong));
				indices.emplace_back(calcIdx(iLat, iLong));
			}
			// wrap band
			indices.emplace_back(calcIdx(iLat + 1, 0));
			indices.emplace_back(calcIdx(iLat + 1, longDiv - 1));
			indices.emplace_back(calcIdx(iLat, 0));
			indices.emplace_back(calcIdx(iLat, 0));
			indices.emplace_back(calcIdx(iLat + 1, longDiv - 1));
			indices.emplace_back(calcIdx(iLat, longDiv - 1));
		}

		// cap fans
		for (IndexBuffer::Type iLong = 0; iLong < longDiv - 1; iLong++)
		{
			// north
			indices.emplace_back(calcIdx(0, iLong + 1));
			indices.emplace_back(calcIdx(0, iLong));
			indices.emplace_back(iNorthPole);
			// south
			indices.emplace_back(iSouthPole);
			indices.emplace_back(calcIdx(latDiv - 2, iLong));
			indices.emplace_back(calcIdx(latDiv - 2, iLong + 1));
		}
		// wrap triangles
		// north
		indices.emplace_back(calcIdx(0, 0));
		indices.emplace_back(calcIdx(0, longDiv - 1));
		indices.emplace_back(iNorthPole);
		// south
		indices.emplace_back(iSouthPole);
		indices.emplace_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.emplace_back(calcIdx(latDiv - 2, 0));

		std::string tag = String::Serialize("UVSphere#{0}#{1}#{2}", radius, latDiv, longDiv);
		return { vertexInput, indices, tag };
	}

	// IndexedTriangleList IndexedTriangleList::IcoSphere(const VertexLayout& layout) {}

	IndexedTriangleList IndexedTriangleList::Cylinder(
		const VertexLayout& layout, int segments, float radius)
	{
		// Top and bottom circles
		IndexedTriangleList topCircle = Circle(layout, segments, radius);
		IndexedTriangleList bottomCircle = topCircle;

		// Translate coordinates so that the middle of the cylinder is at [0, 0, 0],
		// rotate the circles to face upwards/downwards (backface-culling)
		for (uint32_t i = 0; i < topCircle.vertices.Size(); ++i)
		{
			Float3& topCirclePos = topCircle.vertices[i].Get<Float3>(0);
			std::swap(topCirclePos.z, topCirclePos.y);
			topCirclePos.y += 0.5f;

			Float3& bottomCirclePos = bottomCircle.vertices[i].Get<Float3>(0);
			std::swap(bottomCirclePos.z, bottomCirclePos.y);
			bottomCirclePos.y -= 0.5f;

			// Flip circle to face downwards (backface-culling)
			bottomCirclePos.z *= -1;
		}

		// Concatinate both circles to one VertexInput
		IndexedTriangleList finalMesh = Append(topCircle, bottomCircle);
		uint32_t maxIdx = *std::max_element(finalMesh.indices.begin(), finalMesh.indices.end());

		// Generate indices for shell surface and add them to the final mesh
		for (uint32_t i = 0; i < segments; i += 2)
		{
			if (1 - (int)i < 0)
				finalMesh.indices.emplace_back(topCircle.vertices.Size() - (i + 1));
			else
				finalMesh.indices.emplace_back(1);

			finalMesh.indices.emplace_back(topCircle.vertices.Size() + 4 + i);
			finalMesh.indices.emplace_back(topCircle.vertices.Size() + 2 + i);


			if (1 - (int)i < 0)
				finalMesh.indices.emplace_back(topCircle.vertices.Size() - (i + 1));
			else
				finalMesh.indices.emplace_back(1);

			finalMesh.indices.emplace_back(topCircle.vertices.Size() - 3 - i);
			finalMesh.indices.emplace_back(topCircle.vertices.Size() + 4 + i);
		}

		// Do all the uneven vertices
		for (uint32_t i = 1; i < segments; i += 2)
		{
			if (2 - (int)i < 0)
				finalMesh.indices.emplace_back(topCircle.vertices.Size() - (i + 1));
			else
				finalMesh.indices.emplace_back(2);

			finalMesh.indices.emplace_back(topCircle.vertices.Size() + 2 + i);
			finalMesh.indices.emplace_back(topCircle.vertices.Size() + i);

			if (2 - (int)i < 0)
				finalMesh.indices.emplace_back(topCircle.vertices.Size() - (i + 1));
			else
				finalMesh.indices.emplace_back(2);

			finalMesh.indices.emplace_back(topCircle.vertices.Size() - 3 - i);
			finalMesh.indices.emplace_back(topCircle.vertices.Size() + 2 + i);
		}

		return finalMesh;
	}

	IndexedTriangleList IndexedTriangleList::Vector(
		const VertexLayout& layout, const Float3& headPos)
	{
		VertexInput vertexInput(layout);

		vertexInput.Emplace(Float3{ 0.0f, 0.0f, 0.0f });
		vertexInput.Emplace(headPos);

		std::vector<IndexBuffer::Type> indices{ 0, 1 };

		std::string tag = String::Serialize("Vector#{0}#{1}#{2}", headPos.x, headPos.y, headPos.z);
		return { vertexInput, indices, tag };
	}

	IndexedTriangleList IndexedTriangleList::Append(
		const IndexedTriangleList& l1, const IndexedTriangleList& l2)
	{
		RAY_MEXPECTS(l1.vertices.GetLayout() == l2.vertices.GetLayout(),
			"[IndexedTriangleList] Mismatching layouts between two lists.");

		VertexInput vertexInput(l1.vertices.GetLayout());
		vertexInput.Emplace(l1.vertices);
		vertexInput.Emplace(l2.vertices);

		std::vector<IndexBuffer::Type> indices(l1.indices.size() + l2.indices.size());
		memcpy(indices.data(), l1.indices.data(), l1.indices.size() * sizeof(IndexBuffer::Type));

		uint32_t nextInsertID = l1.indices.size();
		uint32_t maxIdx = *std::max_element(l1.indices.begin(), l1.indices.end());
		for (uint32_t i = 0; i < l2.indices.size(); ++i)
		{
			indices[nextInsertID] = l2.indices[i] + maxIdx + 1;
			++nextInsertID;
		}

		return { vertexInput, indices, l1.tag + l2.tag };
	}
}  // namespace At0::Ray
