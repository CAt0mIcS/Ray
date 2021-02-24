#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"


namespace At0::Ray
{
	struct Transform
	{
		Float3 Translation{};

		union
		{
			float Yaw, Roll, Pitch;
			Float3 Rotation;
		};

		Float3 Scale{ 1.0f, 1.0f, 1.0f };

		Transform(Float3 translation, Float3 rotation, Float3 scale)
			: Translation(translation), Rotation(rotation), Scale(scale)
		{
		}
		Transform(Float3 translation, Float3 rotation)
			: Translation(translation), Rotation(rotation)
		{
		}
		Transform(Float3 translation) : Translation(translation) {}
		Transform() = default;

		Matrix ToMatrix() const
		{
			return MatrixScale(Scale) * MatrixRotation(Rotation) * MatrixTranslation(Translation);
		}
	};
}  // namespace At0::Ray
