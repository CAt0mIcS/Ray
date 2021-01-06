#include "RayRender/rlrpch.h"
#include "Camera.h"


namespace At0::Ray
{
	static Vector Transform(FVector v, FMatrix m)
	{
		Vector z = Vector::SplatZ(v);
		Vector y = Vector::SplatY(v);
		Vector x = Vector::SplatX(v);

		Vector result = z * m[2] + m[3];
		result = y * m[1] + result;
		result = x * m[0] + result;

		return result;
	}

	Matrix Camera::GetMatrix() const
	{
		const auto pos = Transform(
			{ 0.0f, 0.0f, -z, 0.0f },
			Matrix::RotationRollPitchYaw(phi, -theta, 0.0f)
		);

		return Matrix::LookAtLH(
			pos, Vector(), Vector(0.0f, 1.0f, 0.0f, 0.0f)
		) * Matrix::RotationRollPitchYaw(pitch, yaw, roll) * Matrix::Translation(x, y, 0.0f);
	}
}



