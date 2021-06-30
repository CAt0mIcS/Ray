#pragma once

#include "Ray/RBase.h"
#include "RayBase/RNonCopyable.h"

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
		operator RrSurface() const { return m_Surface; }

	private:
		RrSurface m_Surface = nullptr;
	};
}  // namespace At0::Ray
