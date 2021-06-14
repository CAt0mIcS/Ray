#pragma once

#include "../Core/RMath.h"
#include "../Graphics/Buffers/RIndexBuffer.h"
#include "../Core/RDynamicVertex.h"


namespace At0::Ray
{
	class Shader;

	struct IndexedTriangleList
	{
		static IndexedTriangleList Plane(const Shader& shader);

		DynamicVertex vertices;
		std::vector<IndexBuffer::Type> indices;
		std::string uniqueTag;
	};
}  // namespace At0::Ray
