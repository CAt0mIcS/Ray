#pragma once

#include "Ray/Utils/RNonCopyable.h"
#include "Ray/RBase.h"

#include <RayRenderer/Core/RCommandPool.h>


namespace At0::Ray
{
	class RAY_EXPORT CommandPool : NonCopyable
	{
	public:
		CommandPool(RrCommandPoolCreateFlags flags = 0);
		~CommandPool();

		operator RrCommandPool() const { return m_CommandPool; }

	private:
		RrCommandPool m_CommandPool = nullptr;
	};
}  // namespace At0::Ray
