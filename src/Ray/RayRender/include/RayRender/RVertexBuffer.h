#pragma once

#include "RRBase.h"


namespace At0::Ray
{
	class RR_API VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create();

		virtual void Bind() = 0;
		virtual ~VertexBuffer() = default;
	};
}