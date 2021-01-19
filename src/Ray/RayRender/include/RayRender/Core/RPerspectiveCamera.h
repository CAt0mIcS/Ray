#pragma once

#include "../RRBase.h"
#include <../../RayUtil/include/RayUtil/RMath.h>


namespace At0::Ray
{
	class RR_API PerspectiveCamera
	{
	public:
		PerspectiveCamera();
		void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

		const Matrix& GetViewMatrix() const;
		const Matrix& GetProjectionMatrix() const;

		void SetPosition(const Float3& pos) { m_Position = pos; m_PositionVector = pos; UpdateMatrix(); }
		void SetRotation(const Float3& rot) { m_Rotation = rot; m_RotationVector = rot; UpdateMatrix(); }
		const Float3& GetPosition() const { return m_Position; }
		const Float3& GetRotation() const { return m_Rotation; }

	private:
		void UpdateMatrix();

	private:
		Float3 m_Position;
		Float3 m_Rotation;
		Vector m_PositionVector;
		Vector m_RotationVector;

		Matrix m_ViewMatrix;
		Matrix m_ProjectionMatrix;
	};
}