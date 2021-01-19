#include "../RRpch.h"
#include "RPerspectiveCamera.h"


namespace At0::Ray
{
	PerspectiveCamera::PerspectiveCamera()
		: m_Position{ 0.0f }, m_Rotation{ 0.0f }
	{
		m_PositionVector = m_Position;
		m_RotationVector = m_Rotation;

		UpdateMatrix();
	}

	void PerspectiveCamera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
	{
		float fovRadians = (fovDegrees / 360.0f) * Constants::RAY_2PI;
		m_ProjectionMatrix = Matrix::PerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
	}

	const Matrix& PerspectiveCamera::GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

	const Matrix& PerspectiveCamera::GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

	void PerspectiveCamera::UpdateMatrix()
	{
		//Calculate Camera3D rotation matrix
		Matrix camRotationMatrix = Matrix::RotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
		//Calculate unit vector of cam target based off Camera3D forward value transformed by cam rotation matrix
		Vector camTarget = Vector3TransformCoord(Constants::DefaultForwardVector, camRotationMatrix);
		//Adjust cam target to be offset by the Camera3D's current position
		camTarget += m_PositionVector;
		//Calculate up direction based on current rotation
		Vector upDir = Vector3TransformCoord(Constants::DefaultUpVector, camRotationMatrix);
		//Rebuild view matrix
		m_ViewMatrix = Matrix::LookAtLH(m_PositionVector, camTarget, upDir);
	}
}


