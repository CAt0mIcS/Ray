#pragma once

#include "RMath.h"
#include "RGlobalDefines.h"

#include <string>


namespace At0::Ray
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent()
			: Tag("New Entity") {}

		TagComponent(std::string_view tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		Float3 Scale;
		Float3 Translation;
		Quaternion Rotation;

		TransformComponent()
			: Scale{ 1.0f, 1.0f, 1.0f }, Translation{ 0.0f, 0.0f, 0.0f }, Rotation{ Quaternion::Identity() } {}

		TransformComponent(const Float3& scale, const Float3& translation, FQuaternion rotation)
			: Scale(scale), Translation(translation), Rotation{ rotation } {}

		TransformComponent(const Float3& scale, const Float3& translation, float pitch, float yaw, float roll)
			: Scale(scale), Translation(translation), Rotation{ Quaternion::RotationRollPitchYaw(pitch, yaw, roll) } {}

		Ray::Matrix ToMatrix() const
		{
			return Ray::Matrix::Scaling(Scale) *
				Ray::Matrix::RotationFromQuaternion(Rotation) *
				Ray::Matrix::Translation(Translation);
		}
	};
}
