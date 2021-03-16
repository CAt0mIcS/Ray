#pragma once

#include "../../RBase.h"
#include "../../Core/RTime.h"


namespace At0::Ray
{
	class CommandBuffer;

	class RAY_EXPORT SubRenderer
	{
	public:
		SubRenderer() = default;
		virtual ~SubRenderer() = default;

		virtual void Update(Delta ts) = 0;
		virtual void Bind(const CommandBuffer& cmdBuff) const = 0;
	};
}  // namespace At0::Ray
