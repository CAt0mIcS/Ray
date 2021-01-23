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
		m_Yaw = WrapAngle(m_Yaw + dx * m_RotationSpeed);
		m_Pitch = std::clamp(m_Pitch + dy * m_RotationSpeed, 0.995f * -Constants::PI / 2.0f, 0.995f * Constants::PI / 2.0f);
	}

	void Camera::Translate(Float3 translation)
	{
		StoreFloat3(&translation, Vector3Transform(
			LoadFloat3(&translation),
			Matrix::RotationRollPitchYaw(m_Pitch, m_Yaw, 0.0f) *
			Matrix::Scaling(m_MovementSpeed, m_MovementSpeed, m_MovementSpeed)
		));

		m_Pos += translation;
	}

	void Camera::Reset()
	{
		m_Pos = { 0.0f, 0.0f, 0.0f };
		m_Pitch = 0.0f;
		m_Yaw = 0.0f;
	}

	Camera::Camera()
		: m_MovementSpeed(10.0f), m_RotationSpeed(0.004f)
	{
		Reset();
	}
}