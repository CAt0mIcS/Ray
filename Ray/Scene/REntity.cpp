#include "Rpch.h"
#include "REntity.h"

#include "RScene.h"
#include "Components/RHierachyComponent.h"


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

	void Entity::SetParent(Entity parent) {}

	bool Entity::HasParent() const
	{
		HierachyComponent* pComponent = m_Registry->try_get<HierachyComponent>(m_EntityHandle);
		return pComponent == nullptr || pComponent->GetParent() == Entity::Null ? false : true;
	}

	Entity Entity::GetParent() const
	{
		RAY_MEXPECTS(HasParent(), "[Entity] Does not have a parent");
		return Get<HierachyComponent>().GetParent();
	}

	const std::vector<Entity>& Entity::GetChildren() const
	{
		RAY_MEXPECTS(Has<HierachyComponent>(),
			"[Entity] Does not have a HierachyComponent to get children.");
		return Get<HierachyComponent>().GetChildren();
	}

	constexpr bool Entity::operator==(const Entity& other) const
	{
		return m_Registry == other.m_Registry && m_EntityHandle == other.m_EntityHandle;
	}

	constexpr bool Entity::operator!=(const Entity& other) const { return !(*this == other); }

	std::ostream& operator<<(std::ostream& os, Entity e) { return os << (int)e.m_EntityHandle; }
}  // namespace At0::Ray
