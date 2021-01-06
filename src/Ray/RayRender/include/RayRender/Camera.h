#pragma once

#include "RlRBase.h"

#include <../../RayMath/include/RayMath/RMatrix.h>


namespace At0::Ray
{
	class RR_API Camera
	{
	public:
		Matrix GetMatrix() const;

	public:
		float x = 0.0f;
		float y = 0.0f;
		float z = 20.0f;
		float theta = 0.0f;
		float phi = 0.0f;
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
	};
}