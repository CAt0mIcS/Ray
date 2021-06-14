#include "Rpch.h"
#include "RTagComponent.h"

namespace At0::Ray
{
	TagComponent::TagComponent(Entity entity, std::string_view tag) : Component(entity), Tag(tag) {}
}  // namespace At0::Ray
