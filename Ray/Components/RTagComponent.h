#pragma once

#include "RComponent.h"

#include <string>


namespace At0::Ray
{
	struct RAY_EXPORT TagComponent : public Component
	{
		TagComponent(Entity entity, std::string_view tag = "Entity");

		std::string Tag;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(TagComponent)
