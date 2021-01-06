#pragma once

#include "RMathInternals.h"
#include "RFloat4.h"

#include <cmath>


namespace At0::Ray
{
	struct Quaternion : public Float4
	{
		// -------------------------------------------------------------------
		// Helper Functions
		void ToEulerAngles(float* pitch, float* yaw, float* roll) const;
		static Quaternion RotationRollPitchYaw(float pitch, float yaw, float roll);

	};

	inline void Quaternion::ToEulerAngles(float* pitch, float* yaw, float* roll) const
	{
		float sinr_cosp = 2 * (w * x + y * z);
		float cosr_cosp = 1 - 2 * (x * x + y * y);
		*roll = std::atan2(sinr_cosp, cosr_cosp);

		float sinp = 2 * (w * y - z * x);
		if (std::abs(sinp) >= 1)
			*pitch = std::copysign(Constants::PIDIV2, sinp);
		else
			*pitch = std::asin(sinp);

		float siny_cosp = 2 * (w * z + x * y);
		float cosy_cosp = 1 - 2 * (y * y + z * z);
		*yaw = std::atan2(siny_cosp, cosy_cosp);
	}

	inline Quaternion Quaternion::RotationRollPitchYaw(float pitch, float yaw, float roll)
	{
		double cy = cos(yaw * 0.5f);
		double sy = sin(yaw * 0.5f);
		double cp = cos(pitch * 0.5f);
		double sp = sin(pitch * 0.5f);
		double cr = cos(roll * 0.5f);
		double sr = sin(roll * 0.5f);

		Quaternion quat;
		quat.w = cr * cp * cy + sr * sp * sy;
		quat.x = sr * cp * cy - cr * sp * sy;
		quat.y = cr * sp * cy + sr * cp * sy;
		quat.z = cr * cp * sy - sr * sp * cy;

		return quat;
	}
}