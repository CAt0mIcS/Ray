#pragma once

namespace At0::Ray
{
	class CommandBuffer;

	class SharedBindable
	{
	public:
		virtual ~SharedBindable() = default;
		virtual void CmdBind(const CommandBuffer& cmdBuff) const = 0;

	protected:
		SharedBindable() = default;
	};
}  // namespace At0::Ray
