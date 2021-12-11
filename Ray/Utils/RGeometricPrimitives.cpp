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
}  // namespace At0::Ray
