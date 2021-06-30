#pragma once

#include "../RBase.h"


namespace At0::Ray
{
	class RAY_EXPORT NonCopyable
	{
	public:
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;

	protected:
		NonCopyable() = default;
	};
}  // namespace At0::Ray
