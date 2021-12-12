#include "RFrustum.h"


namespace At0::Ray
{
	void Frustum::Update(const Matrix& matrix)
	{
		Planes[Left].x = matrix[0].w + matrix[0].x;
		Planes[Left].y = matrix[1].w + matrix[1].x;
		Planes[Left].z = matrix[2].w + matrix[2].x;
		Planes[Left].w = matrix[3].w + matrix[3].x;

		Planes[Right].x = matrix[0].w - matrix[0].x;
		Planes[Right].y = matrix[1].w - matrix[1].x;
		Planes[Right].z = matrix[2].w - matrix[2].x;
		Planes[Right].w = matrix[3].w - matrix[3].x;

		Planes[Top].x = matrix[0].w - matrix[0].y;
		Planes[Top].y = matrix[1].w - matrix[1].y;
		Planes[Top].z = matrix[2].w - matrix[2].y;
		Planes[Top].w = matrix[3].w - matrix[3].y;

		Planes[Bottom].x = matrix[0].w + matrix[0].y;
		Planes[Bottom].y = matrix[1].w + matrix[1].y;
		Planes[Bottom].z = matrix[2].w + matrix[2].y;
		Planes[Bottom].w = matrix[3].w + matrix[3].y;

		Planes[Back].x = matrix[0].w + matrix[0].z;
		Planes[Back].y = matrix[1].w + matrix[1].z;
		Planes[Back].z = matrix[2].w + matrix[2].z;
		Planes[Back].w = matrix[3].w + matrix[3].z;

		Planes[Front].x = matrix[0].w - matrix[0].z;
		Planes[Front].y = matrix[1].w - matrix[1].z;
		Planes[Front].z = matrix[2].w - matrix[2].z;
		Planes[Front].w = matrix[3].w - matrix[3].z;

		for (auto i = 0; i < Planes.size(); i++)
		{
			float length = sqrtf(
				Planes[i].x * Planes[i].x + Planes[i].y * Planes[i].y + Planes[i].z * Planes[i].z);
			Planes[i] /= length;
		}
	}

	bool Frustum::SphereCheck(Float3 pos, float radius) const
	{
		for (auto i = 0; i < Planes.size(); i++)
		{
			if ((Planes[i].x * pos.x) + (Planes[i].y * pos.y) + (Planes[i].z * pos.z) +
					Planes[i].w <=
				-radius)
			{
				return false;
			}
		}
		return true;
	}
}  // namespace At0::Ray
