#include "../RRpch.h"
#include "RCamera.h"


namespace At0::Ray
{
	Matrix Camera::GetMatrix() const
	{
		const Vector forwardBaseVector = Vector::Set(0.0f, 0.0f, 1.0f, 0.0f);

		// apply camera rotation to base vector
		const auto lookVector = Vector3Transform(forwardBaseVector,
			Matrix::RotationRollPitchYaw(m_Pitch, m_Yaw, 0.0f)
		);

		// generate camera transform which is applied to all objects
		const auto camPos = LoadFloat3(&m_Pos);
		const auto camTarget = camPos + lookVector;
		return Matrix::LookAtLH(camPos, camTarget, Vector::Set(0.0f, 1.0f, 0.0f, 0.0f));
	}

	void Camera::SetProjectionValues(float viewWidth, float viewHeight, float nearZ, float farZ)
	{
		//m_Projection = Matrix::PerspectiveFovLH(foVAngle, aspectRatio, nearZ, farZ);
		m_Projection = Matrix::PerspectiveLH(viewWidth, viewHeight, nearZ, farZ);
	}

	void Camera::Rotate(float dx, float dy)
	{
		//m_Yaw =
	}

	void Camera::Translate(Float3 translation)
	{
		StoreFloat3(&translation, Vector3Transform(
			LoadFloat3(&translation),
			Matrix::RotationRollPitchYaw(m_Pitch, m_Yaw, 0.0f) *
			Matrix::Scaling(m_Speed, m_Speed, m_Speed)
		));

		m_Pos += translation;
	}

	Camera::Camera()
		: m_Speed(10.0f)
	{
	}
}