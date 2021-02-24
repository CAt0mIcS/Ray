#pragma once

#include "RBuffer.h"
#include "../Core/RBindable.h"


namespace At0::Ray
{
	class VertexInput;

	class RAY_EXPORT VertexBuffer : public Buffer, public Bindable
	{
	public:
		VertexBuffer(std::string_view tag, const VertexInput& vertices);
		void CmdBind(const CommandBuffer& cmdBuff) const override;

		static std::string GetUID(std::string_view tag, const VertexInput& vertices);
	};
}  // namespace At0::Ray
