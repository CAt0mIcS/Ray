#pragma once

#include "RBuffer.h"
#include "../Core/RSharedBindable.h"


#include "Core/RMath.h"
#include <vector>


namespace At0::Ray
{
	class DynamicVertex;

	class RAY_EXPORT VertexBuffer : public Buffer, public SharedBindable
	{
	public:
		VertexBuffer(std::string_view tag, const DynamicVertex& vertices);
		void CmdBind(const CommandBuffer& cmdBuff) const override;

		static std::string GetUID(std::string_view tag, const DynamicVertex& vertices);
	};
}  // namespace At0::Ray
