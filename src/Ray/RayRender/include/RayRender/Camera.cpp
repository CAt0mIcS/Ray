#include "RayRender/rlrpch.h"
#include "Camera.h"


namespace At0::Ray
{
	DirectX::XMMATRIX Camera::GetMatrix() const
	{
		const auto pos = Vector3Transform(
			{ 0.0f, 0.0f, -z, 0.0f },
			MatrixRotationRollPitchYaw(phi, -theta, 0.0f)
		);

		const auto mat = MatrixLookAtLH(
			pos, VectorZero(), VectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		) * MatrixRotationRollPitchYaw(pitch, yaw, roll) * MatrixTranslation(x, y, 0.0f);

		DirectX::XMMATRIX ret;
		ret.r[0].m128_f32[0] = mat.r[0].f[0];
		ret.r[0].m128_f32[1] = mat.r[0].f[1];
		ret.r[0].m128_f32[2] = mat.r[0].f[2];
		ret.r[0].m128_f32[3] = mat.r[0].f[3];

		ret.r[1].m128_f32[0] = mat.r[1].f[0];
		ret.r[1].m128_f32[1] = mat.r[1].f[1];
		ret.r[1].m128_f32[2] = mat.r[1].f[2];
		ret.r[1].m128_f32[3] = mat.r[1].f[3];

		ret.r[2].m128_f32[0] = mat.r[2].f[0];
		ret.r[2].m128_f32[1] = mat.r[2].f[1];
		ret.r[2].m128_f32[2] = mat.r[2].f[2];
		ret.r[2].m128_f32[3] = mat.r[2].f[3];

		ret.r[3].m128_f32[0] = mat.r[3].f[0];
		ret.r[3].m128_f32[1] = mat.r[3].f[1];
		ret.r[3].m128_f32[2] = mat.r[3].f[2];
		ret.r[3].m128_f32[3] = mat.r[3].f[3];

		return ret;
	}
}



