#include "RayRender/rlrpch.h"
#include "Camera.h"


namespace At0::Ray
{
	Matrix Camera::GetMatrix() const
	{
		const auto pos = Vector3Transform(
			VectorSet(0.0f, 0.0f, -z, 0.0f),
			MatrixRotationRollPitchYaw(phi, -theta, 0.0f)
		);

		return MatrixLookAtLH(
			pos, VectorZero(), VectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		) * MatrixRotationRollPitchYaw(pitch, yaw, roll) * MatrixTranslation(x, y, 0.0f);
	}
}



