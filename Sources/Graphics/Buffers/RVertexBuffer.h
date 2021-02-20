#pragma once

#include "RBuffer.h"
#include "../../Core/RMath.h"

namespace At0::Ray
{
	class RAY_EXPORT VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(const std::vector<Float3>& vertices);
	};
}  // namespace At0::Ray
