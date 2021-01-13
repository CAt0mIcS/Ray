#include "../RRpch.h"
#include "RQuad.h"

#include <RayUtil/RComponents.h>


namespace At0::Ray
{
	Quad::Quad(std::string_view tag)
	{
		AddComponent<TagComponent>(tag);
		AddComponent<TransformComponent>(
			Ray::Float3{ 0.0f },
			Ray::Float3{ 0.0f },
			Ray::Float3{ 0.0f }
		);
	}
}


