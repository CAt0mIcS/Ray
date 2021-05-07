#pragma once

#include "../RBase.h"
#include "RComponent.h"

class Texture2D;

namespace At0::Ray
{
	class RAY_EXPORT Skybox : public Component
	{
	public:
		Skybox(Entity entity);
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Skybox);
