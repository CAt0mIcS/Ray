#pragma once

#include "RRBase.h"
#include <initializer_list>


namespace At0::Ray
{
	class VertexData;

	class RR_API VertexBuffer
	{
	public:
		static Scope<VertexBuffer> Create(const VertexData& data);

		virtual void Bind() = 0;
		virtual ~VertexBuffer() = default;
	};
}