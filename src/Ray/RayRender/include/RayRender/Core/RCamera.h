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
		void SetSpeed(float speed) { m_Speed = speed; }

		const Matrix& GetProjection() const { return m_Projection; }

	protected:
		Camera();

	protected:
		Float3 m_Pos;
		float m_Pitch, m_Yaw;
		float m_Speed;

		Matrix m_Projection;
	};
}