#pragma once

#include "RMath.h"

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
		Float3 Rotation;

		TransformComponent()
			: Scale{ 1.0f, 1.0f, 1.0f }, Translation{ 0.0f, 0.0f, 0.0f }, Rotation{ } {}

		TransformComponent(const Float3& scale, const Float3& translation, const Float3& rotation)
			: Scale(scale), Translation(translation), Rotation(rotation) {}

		Ray::Matrix ToMatrix() const
		{
			return Ray::Matrix::Scaling(Scale.x, Scale.y, Scale.z) *
				Ray::Matrix::Translation(Translation.x, Translation.y, Translation.z) *
				Ray::Matrix::RotationRollPitchYawFromVector({ Rotation.x, Rotation.y, Rotation.z, 0.0f });
		}
	};
}