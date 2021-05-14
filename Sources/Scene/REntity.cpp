#include "Rpch.h"
#include "REntity.h"

#include "RScene.h"

#include "Components/RParentEntity.h"


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

	void Entity::SetParent(Entity parent)
	{
		if (Has<ParentEntity>())
			Get<ParentEntity>().SetParent(parent);
		else
			Emplace<ParentEntity>(parent);
	}

	bool Entity::HasParent() const
	{
		return Has<ParentEntity>() && Get<ParentEntity>().GetParent() != Entity::Null;
	}

	Entity Entity::GetParent() const
	{
		RAY_MEXPECTS(Has<ParentEntity>(), "[Entity] Does not have parent component");
		return Get<ParentEntity>().GetParent();
	}

	constexpr bool Entity::operator==(const Entity& other) const
	{
		return m_Registry == other.m_Registry && m_EntityHandle == other.m_EntityHandle;
	}

	constexpr bool Entity::operator!=(const Entity& other) const { return !(*this == other); }
}  // namespace At0::Ray
