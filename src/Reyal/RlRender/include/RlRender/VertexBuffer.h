#pragma once

#include "RlRBase.h"


namespace At0::Reyal
{
	class RLR_API VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create();

	protected:
		VertexBuffer() = default;
	};
}