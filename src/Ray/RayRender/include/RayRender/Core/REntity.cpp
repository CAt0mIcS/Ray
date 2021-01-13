#include "REntity.h"

namespace At0::Ray
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_Entity(handle), m_Scene(scene)
	{
	}
}


