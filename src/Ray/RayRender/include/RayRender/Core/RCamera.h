#pragma once

#include "../RRBase.h"
#include <../../RayUtil/include/RayUtil/RMath.h>


namespace At0::Ray
{
	class RR_API Camera
	{
	public:
		virtual ~Camera() = default;

		Matrix GetMatrix() const;

		float x = 0.0f;
		float y = 0.0f;
		float z = 20.0f;
		float theta = 0.0f;
		float phi = 0.0f;
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;

		Matrix Projection;

	protected:
		Camera() = default;
	};
}