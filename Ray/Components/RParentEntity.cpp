#include "Rpch.h"
#include "RParentEntity.h"


namespace At0::Ray
{
	ParentEntity::ParentEntity(Entity entity, Entity parent) : Component(entity), m_Parent(parent)
	{
	}
}  // namespace At0::Ray
