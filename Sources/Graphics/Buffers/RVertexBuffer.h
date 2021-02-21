#pragma once

#include "RBuffer.h"
#include "../../Core/RMath.h"

namespace At0::Ray
{
	class VertexInput;

	class RAY_EXPORT VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(const VertexInput& vertices);
	};
}  // namespace At0::Ray
