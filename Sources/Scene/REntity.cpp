#include "Rpch.h"
#include "REntity.h"

#include "RScene.h"


namespace At0::Ray
{
	Entity::Entity(entt::entity handle, entt::registry* registry)
		: m_EntityHandle(handle), m_Registry(registry)
	{
	}

	Entity::Entity(entt::entity handle)
		: m_EntityHandle(handle), m_Registry(&Scene::Get().GetRegistry())
	{
	}
}  // namespace At0::Ray
