#include "REntity.h"

#include "RScene.h"
#include "Components/RHierachyComponent.h"


namespace At0::Ray
{
	Entity Entity::Empty = Entity{};

	Entity::Entity(entt::entity handle, entt::registry* registry)
		: m_EntityHandle(handle), m_Registry(registry)
	{
	}

	Entity::Entity(entt::entity handle)
		: m_EntityHandle(handle), m_Registry(&Scene::Get().GetRegistry())
	{
	}

	void Entity::Destroy()
	{
		RAY_MEXPECTS(Valid(), "[Entity] Cannot get component(s) of null entity.");
		m_Registry->destroy(m_EntityHandle);
		m_EntityHandle = Entity::Null;
	}

	void Entity::SetParent(Entity parent)
	{
		RAY_MEXPECTS(Has<HierachyComponent>(), "[Entity] Does not have a HierachyComponent");
		Get<HierachyComponent>().SetParent(parent);
	}

	void Entity::AddChild(Entity child)
	{
		RAY_MEXPECTS(Has<HierachyComponent>(), "[Entity] Does not have a HierachyComponent");
		RAY_MEXPECTS(!HasChild(child), "[Entity] Already has child");
		Get<HierachyComponent>().AddChild(child);
	}

	void Entity::AddChildren(const std::vector<Entity>& children)
	{
		RAY_MEXPECTS(Has<HierachyComponent>(), "[Entity] Does not have a HierachyComponent");
		Get<HierachyComponent>().AddChildren(children);
	}

	bool Entity::HasParent() const
	{
		RAY_MEXPECTS(Valid(), "[Entity] Cannot get component(s) of null entity.");
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

	bool Entity::HasChild(Entity child) const
	{
		RAY_MEXPECTS(Has<HierachyComponent>(), "[Entity] Does not have a HierachyComponent.");
		return Get<HierachyComponent>().HasChild(child);
	}

	void Entity::RemoveChild(Entity child)
	{
		RAY_MEXPECTS(Has<HierachyComponent>(), "[Entity] Does not have a HierachyComponent.");
		Get<HierachyComponent>().RemoveChild(child);
	}

	constexpr bool Entity::operator==(const Entity& other) const
	{
		return m_Registry == other.m_Registry && m_EntityHandle == other.m_EntityHandle;
	}

	constexpr bool Entity::operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

	std::ostream& operator<<(std::ostream& os, Entity e)
	{
		return os << (int)e.m_EntityHandle;
	}
}  // namespace At0::Ray
