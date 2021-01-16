#include "REntity.h"

namespace At0::Ray
{
	Entity::Entity(entt::entity handle, entt::registry* registry)
		: m_Entity(handle), m_Registry(registry)
	{
	}
}


