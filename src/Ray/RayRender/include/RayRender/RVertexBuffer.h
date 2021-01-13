#pragma once

#include "RRBase.h"
#include <vector>


namespace At0::Ray
{
	// Inherit your Vertex class from this one
	class RR_API Vertex
	{
	protected:
		Vertex() = default;
		virtual ~Vertex() = default;
	};

	class RR_API VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create(const std::vector<Vertex>& data);

		virtual void Bind() = 0;
		virtual ~VertexBuffer() = default;
	};
}