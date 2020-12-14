#pragma once

#include "RlRBase.h"
#include <DirectXMath.h>

namespace At0::Ray
{
	class RR_API Camera
	{
	public:
		DirectX::XMMATRIX GetMatrix() const;

	public:
		float r = 20.0f, theta = 0.0f, phi = 0.0f, roll = 0.0f, pitch = 0.0f, yaw = 0.0f;
	};
}