#pragma once

#include "Ray/RBase.h"
#include "Ray/Core/RMath.h"


namespace At0::Ray
{
	struct RAY_EXPORT Frustum
	{
	public:
		enum Side
		{
			Left = 0,
			Right,
			Top,
			Bottom,
			Back,
			Front
		};
		std::array<Float4, 6> Planes;

		void Update(const Matrix& matrix);

		bool SphereCheck(Float3 pos, float radius) const;
	};
}  // namespace At0::Ray
