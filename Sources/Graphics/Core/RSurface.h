#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>


namespace At0::Ray
{
	class RAY_EXPORT Surface : NonCopyable
	{
	public:
		Surface();
		~Surface();

		operator const VkSurfaceKHR&() const { return m_Surface; }

	private:
		VkSurfaceKHR m_Surface;
	};
}  // namespace At0::Ray
