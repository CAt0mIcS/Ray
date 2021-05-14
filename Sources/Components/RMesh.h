#pragma once

#include "../RBase.h"
#include "RComponent.h"


namespace At0::Ray
{
	class RAY_EXPORT Mesh : public Component
	{
	public:
		Mesh(Entity entity);
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Mesh);
