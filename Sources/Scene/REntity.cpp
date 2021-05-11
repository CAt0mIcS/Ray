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

	constexpr bool Entity::operator==(const Entity& other) const
	{
		return m_Registry == other.m_Registry && m_EntityHandle == other.m_EntityHandle;
	}

	constexpr bool Entity::operator!=(const Entity& other) const { return !(*this == other); }
}  // namespace At0::Ray
