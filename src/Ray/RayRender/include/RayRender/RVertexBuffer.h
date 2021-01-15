#pragma once

#include "RRBase.h"
#include <initializer_list>


namespace At0::Ray
{
	struct Vertex
	{
		struct
		{
			float x, y, z;
		} pos;
	};

	class RR_API VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create(std::initializer_list<Vertex> data);

		virtual void Bind() = 0;
		virtual ~VertexBuffer() = default;
	};
}