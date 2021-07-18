#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"

#include <vulkan/vulkan_core.h>
#include <RayRenderer/Core/RSurface.h>


namespace At0::Ray
{
	class RAY_EXPORT Surface : NonCopyable
	{
	public:
		Surface();
		~Surface();

		operator VkSurfaceKHR() const { return (VkSurfaceKHR)m_Surface; }
		operator RrSurfaceKHR() const { return m_Surface; }

	private:
		RrSurfaceKHR m_Surface = nullptr;
	};
}  // namespace At0::Ray
