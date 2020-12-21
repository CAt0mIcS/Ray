#pragma once



namespace At0::Ray
{
	struct TranslationComponent
	{
		float x, y, z;

		TranslationComponent(float x, float y, float z)
			: x(x), y(y), z(z) {}
	};

	struct ScaleComponent
	{
		float x, y, z;

		ScaleComponent(float x, float y, float z)
			: x(x), y(y), z(z) {}
	};

	struct RotationComponent
	{
		RotationComponent() = default;
	};

	struct TransformComponent
	{
		TranslationComponent Translation;
		ScaleComponent Scale;
		RotationComponent Rotation;

		TransformComponent() = default;
	};
}