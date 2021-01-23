#pragma once

#include "../RRBase.h"
#include <../../RayUtil/include/RayUtil/RMath.h>


namespace At0::Ray
{
	class RR_API Camera
	{
	public:
		virtual ~Camera() = default;

		Matrix GetMatrix() const;

		//float x = 0.0f;
		//float y = 0.0f;
		//float z = 20.0f;
		//float theta = 0.0f;
		//float phi = 0.0f;
		//float roll = 0.0f;
		//float pitch = 0.0f;
		//float yaw = 0.0f;

		//Matrix Projection;

		void SetProjectionValues(float viewWidth, float viewHeight, float nearZ, float farZ);

		void Rotate(float dx, float dy);
		void Translate(Float3 translation);
		void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }
		void SetRotationSpeed(float speed) { m_RotationSpeed = speed; }

		const Matrix& GetProjection() const { return m_Projection; }

		void Reset();

	protected:
		Camera();

	protected:
		Float3 m_Pos;
		float m_Pitch, m_Yaw;
		float m_MovementSpeed, m_RotationSpeed;

		Matrix m_Projection;
	};
}