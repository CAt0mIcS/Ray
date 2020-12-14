#include "RayRender/rlrpch.h"
#include "Camera.h"


namespace At0::Ray
{
	DirectX::XMMATRIX Camera::GetMatrix() const
	{
		const auto pos = DirectX::XMVector3Transform(
			DirectX::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
			DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
		);

		return DirectX::XMMatrixLookAtLH(
			pos, DirectX::XMVectorZero(), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		) * DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	}
}



