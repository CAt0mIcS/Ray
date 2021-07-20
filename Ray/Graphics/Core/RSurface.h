#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"

#include <RayRenderer/Core/RSurface.h>


namespace At0::Ray
{
	class RAY_EXPORT Surface : NonCopyable
	{
	public:
		Surface();
		~Surface();

		operator RrSurfaceKHR() const { return m_Surface; }

	private:
		RrSurfaceKHR m_Surface = nullptr;
	};
}  // namespace At0::Ray
