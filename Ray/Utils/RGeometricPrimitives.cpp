#include "RGeometricPrimitives.h"
#include "Core/RMath.h"


namespace At0::Ray
{
	IndexedTriangleList IndexedTriangleList::Triangle(const Shader& shader)
	{
		DynamicVertex vertex(shader);

		bool hasPos = vertex.Has(AttributeMap<AttributeType::Position>::Semantic);
		bool hasUV = vertex.Has(AttributeMap<AttributeType::UV>::Semantic);
		bool hasNormal = vertex.Has(AttributeMap<AttributeType::Normal>::Semantic);
		bool hasTangent = vertex.Has(AttributeMap<AttributeType::Tangent>::Semantic);

		RAY_MEXPECTS(!hasNormal && !hasTangent && !hasUV,
			"[IndexedTriangleList] Normals, tangents and "
			"UV-Coordinates not supported for triangle yet");

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, -0.5f, 0.0f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, -0.5f, 0.0f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.0f, 0.5f, 0.0f };

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2 };

		return { vertex, indices,
			String::Serialize(
				"Triangle#012230#{0}#{1}#{2}#{3}", hasPos, hasUV, hasNormal, hasTangent) };
	}

	IndexedTriangleList IndexedTriangleList::Plane(const Shader& shader)
	{
		DynamicVertex vertex(shader);

		bool hasPos = vertex.Has(AttributeMap<AttributeType::Position>::Semantic);
		bool hasUV = vertex.Has(AttributeMap<AttributeType::UV>::Semantic);
		bool hasNormal = vertex.Has(AttributeMap<AttributeType::Normal>::Semantic);
		bool hasTangent = vertex.Has(AttributeMap<AttributeType::Tangent>::Semantic);

		RAY_MEXPECTS(!hasNormal && !hasTangent,
			"[IndexedTriangleList] Normals and tangents not supported for plane yet");

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, -0.5f, 0.0f };
		if (hasUV)
			vertex[AttributeMap<AttributeType::UV>::Semantic] = Float2{ 1.0f, 0.0f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, -0.5f, 0.0f };
		if (hasUV)
			vertex[AttributeMap<AttributeType::UV>::Semantic] = Float2{ 0.0f, 0.0f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, 0.5f, 0.0f };
		if (hasUV)
			vertex[AttributeMap<AttributeType::UV>::Semantic] = Float2{ 0.0f, 1.0f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, 0.5f, 0.0f };
		if (hasUV)
			vertex[AttributeMap<AttributeType::UV>::Semantic] = Float2{ 1.0f, 1.0f };

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2, 2, 3, 0 };
		return { vertex, indices,
			String::Serialize(
				"Plane#012230#{0}#{1}#{2}#{3}", hasPos, hasUV, hasNormal, hasTangent) };
	}

	IndexedTriangleList IndexedTriangleList::HalfCircle(
		const Shader& shader, int segments, float radius)
	{
		DynamicVertex vertex(shader);

		bool hasPos = vertex.Has(AttributeMap<AttributeType::Position>::Semantic);
		bool hasUV = vertex.Has(AttributeMap<AttributeType::UV>::Semantic);
		bool hasNormal = vertex.Has(AttributeMap<AttributeType::Normal>::Semantic);
		bool hasTangent = vertex.Has(AttributeMap<AttributeType::Tangent>::Semantic);

		RAY_MEXPECTS(!hasNormal && !hasTangent && !hasUV,
			"[IndexedTriangleList] UV, normals and tangents not supported for half circle yet");

		RAY_MEXPECTS(segments % 2 == 0,
			"[IndexedTriangleList] Cannot create half circle with {0} segments because this value "
			"is "
			"uneven",
			segments);

		// circle middle
		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.0f, 0.0f, 0.0f };

		const Float4 base = Float4{ 0.0f, 0.0f, radius, 0.0f };
		const float lattitudeAngle = Math::PI<float> / segments;

		for (int segment = 0; segment <= segments; ++segment)
		{
			Float3 calculatedPos = base * glm::rotate(Matrix(1.0f), lattitudeAngle * segment,
											  Float3(0.0f, -1.0f, 0.0f));

			// Rotate circle to face camera instead of floor
			// std::swap(calculatedPos.z, calculatedPos.x);
			// std::swap(calculatedPos.z, calculatedPos.y);

			vertex.BeginVertex();
			if (hasPos)
				vertex[AttributeMap<AttributeType::Position>::Semantic] = calculatedPos;
		}

		std::vector<IndexBuffer::Type> indices;
		for (int segment = 0; segment <= segments; ++segment)
		{
			// circle middle
			indices.emplace_back(0);

			indices.emplace_back(segment);
			indices.emplace_back(segment + 1);
		}

		return { vertex, indices,
			String::Serialize("HalfCircle#{0}#{1}#{2}#{3}#{4}#{5}", hasPos, hasUV, hasNormal,
				hasTangent, segments, radius) };
	}

	IndexedTriangleList IndexedTriangleList::Circle(
		const Shader& shader, int segments, float radius)
	{
		DynamicVertex vertex(shader);

		bool hasPos = vertex.Has(AttributeMap<AttributeType::Position>::Semantic);
		bool hasUV = vertex.Has(AttributeMap<AttributeType::UV>::Semantic);
		bool hasNormal = vertex.Has(AttributeMap<AttributeType::Normal>::Semantic);
		bool hasTangent = vertex.Has(AttributeMap<AttributeType::Tangent>::Semantic);

		RAY_MEXPECTS(!hasNormal && !hasTangent && !hasUV,
			"[IndexedTriangleList] UV, normals and tangents not supported for circle yet");

		RAY_MEXPECTS(segments % 2 == 0,
			"[IndexedTriangleList] Cannot create circle with {0} segments because this value is "
			"uneven",
			segments);
		// Split segments for half circle
		segments /= 2;

		// circle middle
		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.0f, 0.0f, 0.0f };

		const Float4 base = Float4{ 0.0f, 0.0f, radius, 0.0f };
		const float lattitudeAngle = Math::PI<float> / segments;

		for (int segment = 0; segment <= segments; ++segment)
		{
			Float3 calculatedPos = base * glm::rotate(Matrix(1.0f), lattitudeAngle * segment,
											  Float3(0.0f, -1.0f, 0.0f));

			// Rotate circle to face camera instead of floor
			// std::swap(calculatedPos.z, calculatedPos.y);

			vertex.BeginVertex();
			if (hasPos)
				vertex[AttributeMap<AttributeType::Position>::Semantic] = calculatedPos;

			vertex.BeginVertex();
			if (hasPos)
				vertex[AttributeMap<AttributeType::Position>::Semantic] = -calculatedPos;
		}

		std::vector<IndexBuffer::Type> indices;
		for (uint32_t segment = 1; segment <= segments * 2; ++segment)
		{
			indices.emplace_back(0);
			indices.emplace_back(segment);
			indices.emplace_back(segment + 2);
		}

		return { vertex, indices,
			String::Serialize("Circle#{0}#{1}#{2}#{3}#{4}#{5}", hasPos, hasUV, hasNormal,
				hasTangent, segments, radius) };
	}

	IndexedTriangleList IndexedTriangleList::Cube(const Shader& shader)
	{
		DynamicVertex vertex(shader);

		bool hasPos = vertex.Has(AttributeMap<AttributeType::Position>::Semantic);
		bool hasUV = vertex.Has(AttributeMap<AttributeType::UV>::Semantic);
		bool hasNormal = vertex.Has(AttributeMap<AttributeType::Normal>::Semantic);
		bool hasTangent = vertex.Has(AttributeMap<AttributeType::Tangent>::Semantic);

		RAY_MEXPECTS(!hasNormal && !hasTangent && !hasUV,
			"[IndexedTriangleList] UV, normals and tangents not supported for cube yet");

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, -0.5f, -0.5f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, -0.5f, -0.5f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, 0.5f, -0.5f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, 0.5f, -0.5f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, -0.5f, 0.5f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, -0.5f, 0.5f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ -0.5f, 0.5f, 0.5f };

		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.5f, 0.5f, 0.5f };

		std::vector<IndexBuffer::Type> indices{ 0, 2, 1, 2, 3, 1, 1, 3, 5, 3, 7, 5, 2, 6, 3, 3, 6,
			7, 4, 5, 7, 4, 7, 6, 0, 4, 2, 2, 4, 6, 0, 1, 4, 1, 5, 4 };

		return { vertex, indices,
			String::Serialize("Cube#021231135375263367457476042246014154#{0}#{1}#{2}#{3}", hasPos,
				hasUV, hasNormal, hasTangent) };
	}

	IndexedTriangleList IndexedTriangleList::UVSphere(
		const Shader& shader, float radius, int latDiv, int longDiv)
	{
		const Float4 base = Float4{ 0.0f, 0.0f, radius, 0.0f };
		const float lattitudeAngle = Math::PI<float> / latDiv;
		const float longitudeAngle = 2.0f * Math::PI<float> / longDiv;

		DynamicVertex vertex(shader);

		bool hasPos = vertex.Has(AttributeMap<AttributeType::Position>::Semantic);
		bool hasUV = vertex.Has(AttributeMap<AttributeType::UV>::Semantic);
		bool hasNormal = vertex.Has(AttributeMap<AttributeType::Normal>::Semantic);
		bool hasTangent = vertex.Has(AttributeMap<AttributeType::Tangent>::Semantic);

		RAY_MEXPECTS(!hasNormal && !hasTangent && !hasUV,
			"[IndexedTriangleList] UV, normals and tangents not supported for UVSphere yet");

		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			Float4 latBase =
				base * glm::rotate(Matrix(1.0f), lattitudeAngle * iLat, Float3(1.0f, 0.0f, 0.0f));

			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				Float3 calculatedPos = latBase * glm::rotate(Matrix(1.0f), longitudeAngle * iLong,
													 Float3(0.0f, 0.0f, 1.0f));

				vertex.BeginVertex();
				if (hasPos)
					vertex[AttributeMap<AttributeType::Position>::Semantic] = calculatedPos;
			}
		}

		// add the cap vertices
		const uint16_t iNorthPole = (uint16_t)vertex.Size();
		{
			Float3 northPos = base;
			vertex.BeginVertex();
			if (hasPos)
				vertex[AttributeMap<AttributeType::Position>::Semantic] = northPos;
		}
		const uint16_t iSouthPole = (uint16_t)vertex.Size();
		{
			Float3 southPos = -base;
			vertex.BeginVertex();
			if (hasPos)
				vertex[AttributeMap<AttributeType::Position>::Semantic] = southPos;
		}

		const auto calcIdx = [latDiv, longDiv](IndexBuffer::Type iLat, IndexBuffer::Type iLong)
		{ return iLat * longDiv + iLong; };

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

		return { vertex, indices,
			String::Serialize("UVSphere#{0}#{1}#{2}#{3}#{4}#{5}#{6}", hasPos, hasUV, hasNormal,
				hasTangent, radius, latDiv, longDiv) };
	}

	// IndexedTriangleList IndexedTriangleList::IcoSphere(const VertexLayout& layout) {}

	IndexedTriangleList IndexedTriangleList::Cylinder(
		const Shader& shader, int segments, float radius)
	{
		// Top and bottom circles
		IndexedTriangleList topCircle = Circle(shader, segments, radius);
		IndexedTriangleList bottomCircle = topCircle;

		// Translate coordinates so that the middle of the cylinder is at [0, 0, 0],
		// rotate the circles to face upwards/downwards (backface-culling)
		for (uint32_t i = 0; i < topCircle.vertices.Size(); ++i)
		{
			Float3& topCirclePos =
				topCircle.vertices[i].Get<Float3>(AttributeMap<AttributeType::Position>::Semantic);
			// std::swap(topCirclePos.z, topCirclePos.y);
			topCirclePos.y += 0.5f;

			Float3& bottomCirclePos = bottomCircle.vertices[i].Get<Float3>(
				AttributeMap<AttributeType::Position>::Semantic);
			// std::swap(bottomCirclePos.z, bottomCirclePos.y);
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

	IndexedTriangleList IndexedTriangleList::Cone(const Shader& shader, int segments, float radius)
	{
		DynamicVertex vertex(shader);

		bool hasPos = vertex.Has(AttributeMap<AttributeType::Position>::Semantic);
		bool hasUV = vertex.Has(AttributeMap<AttributeType::UV>::Semantic);
		bool hasNormal = vertex.Has(AttributeMap<AttributeType::Normal>::Semantic);
		bool hasTangent = vertex.Has(AttributeMap<AttributeType::Tangent>::Semantic);

		RAY_MEXPECTS(!hasNormal && !hasTangent && !hasUV,
			"[IndexedTriangleList] UV, normals and tangents not supported for cone yet");

		RAY_MEXPECTS(segments % 2 == 0,
			"[IndexedTriangleList] Cannot create circle with {0} segments because this value is "
			"uneven",
			segments);
		segments /= 2;

		// Tip of cone
		vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.f, 1.f, 0.f };

		const Float4 base = Float4{ 0.0f, 0.0f, radius, 0.0f };
		const float lattitudeAngle = Math::PI<float> / segments;

		for (int segment = 0; segment <= segments; ++segment)
		{
			Float3 calculatedPos = base * glm::rotate(Matrix(1.0f), lattitudeAngle * segment,
											  Float3(0.0f, -1.0f, 0.0f));

			vertex.BeginVertex();
			if (hasPos)
				vertex[AttributeMap<AttributeType::Position>::Semantic] = calculatedPos;

			vertex.BeginVertex();
			if (hasPos)
				vertex[AttributeMap<AttributeType::Position>::Semantic] = -calculatedPos;
		}

		// Circle middle
		uint32_t circleMiddleVertexID = vertex.BeginVertex();
		if (hasPos)
			vertex[AttributeMap<AttributeType::Position>::Semantic] = Float3{ 0.f, 0.f, 0.f };

		std::vector<IndexBuffer::Type> indices;
		for (uint32_t segment = 1; segment <= segments * 2; ++segment)
		{
			indices.emplace_back(0);
			indices.emplace_back(segment);
			indices.emplace_back(segment + 2);

			indices.emplace_back(circleMiddleVertexID);
			indices.emplace_back(segment + 2);
			indices.emplace_back(segment);
		}

		return { vertex, indices,
			String::Serialize("Cone#{0}#{1}#{2}#{3}#{4}#{5}", hasPos, hasUV, hasNormal, hasTangent,
				segments, radius) };
	}

	IndexedTriangleList IndexedTriangleList::Append(
		const IndexedTriangleList& first, const IndexedTriangleList& last)
	{
		RAY_MEXPECTS(first.vertices.GetLayout() == last.vertices.GetLayout() &&
						 first.vertices.SizeVertex() == last.vertices.SizeVertex(),
			"[IndexedTriangleList] Mismatching layouts between two lists.");

		DynamicVertex vertex(first.vertices.GetLayout());
		vertex.AddAll(first.vertices);
		vertex.AddAll(last.vertices);

		std::vector<IndexBuffer::Type> indices(first.indices.size() + last.indices.size());
		memcpy(
			indices.data(), first.indices.data(), first.indices.size() * sizeof(IndexBuffer::Type));

		uint32_t nextInsertID = first.indices.size();
		uint32_t maxIdx = *std::max_element(first.indices.begin(), first.indices.end());
		for (uint32_t i = 0; i < last.indices.size(); ++i)
		{
			indices[nextInsertID] = last.indices[i] + maxIdx + 1;
			++nextInsertID;
		}

		return { vertex, indices, first.uniqueTag + last.uniqueTag };
	}
}  // namespace At0::Ray
