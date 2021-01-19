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