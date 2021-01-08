#include "RayRender/rlrpch.h"
#include "Camera.h"


namespace At0::Ray
{
	Matrix Camera::GetMatrix() const
	{
		const auto pos = Vector3Transform(
			{ 0.0f, 0.0f, -z, 0.0f },
			Matrix::RotationRollPitchYaw(phi, -theta, 0.0f)
		);

		return Matrix::LookAtLH(
			pos, Vector::Zero(), Vector::Set(0.0f, 1.0f, 0.0f, 0.0f)
		) * Matrix::RotationRollPitchYaw(pitch, yaw, roll) * Matrix::Translation(x, y, 0.0f);
	}
}



