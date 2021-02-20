#pragma once

#include "../RBase.h"


namespace At0::Ray
{
	class RAY_EXPORT NonCopyable
	{
	public:
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;

		NonCopyable(NonCopyable&&) = default;
		NonCopyable& operator=(NonCopyable&&) = default;

	protected:
		NonCopyable() = default;
	};
}  // namespace At0::Ray
