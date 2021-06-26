#pragma once

#include "Ray/RBase.h"
#include "RayBase/RNonCopyable.h"

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
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
	};
}  // namespace At0::Ray
