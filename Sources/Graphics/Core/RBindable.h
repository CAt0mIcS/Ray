#pragma once

#include "../../RBase.h"


namespace At0::Ray
{
	class CommandBuffer;

	class RAY_EXPORT Bindable
	{
	public:
		virtual ~Bindable() = default;
		virtual void CmdBind(const CommandBuffer& cmdBuff) const = 0;

	protected:
		Bindable() = default;
	};
}  // namespace At0::Ray
