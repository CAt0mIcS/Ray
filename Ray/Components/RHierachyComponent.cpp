#include "RHierachyComponent.h"

#define RAY_AUTOMATIC_CHILD_PARENT_RELATION_EMPLACEMENT 0


namespace At0::Ray
{
	HierachyComponent::HierachyComponent(Entity entity) : Component(entity) {}

	void HierachyComponent::AddChild(Entity child)
	{
		RAY_MEXPECTS(!HasChild(child),
			"[HierachyComponent] Entity {0} already has entity {1} as child", GetEntity(), child);
		m_Children.emplace_back(child);

#if RAY_AUTOMATIC_CHILD_PARENT_RELATION_EMPLACEMENT
		if (child.Has<HierachyComponent>())
			child.Get<HierachyComponent>().SetParent(GetEntity());
		else
			child.Emplace<HierachyComponent>().SetParent(GetEntity());
#endif
	}

	void HierachyComponent::AddChildren(const std::vector<Entity>& children)
	{
		m_Children.insert(m_Children.end(), children.begin(), children.end());

#if RAY_AUTOMATIC_CHILD_PARENT_RELATION_EMPLACEMENT
		for (Entity child : children)
		{
			if (child.Has<HierachyComponent>())
				child.Get<HierachyComponent>().SetParent(GetEntity());
			else
				child.Emplace<HierachyComponent>().SetParent(GetEntity());
		}
#endif
	}

	void HierachyComponent::SetParent(Entity parent)
	{
		m_Parent = parent;

#if RAY_AUTOMATIC_CHILD_PARENT_RELATION_EMPLACEMENT
		if (m_Parent.Has<HierachyComponent>())
			m_Parent.Get<HierachyComponent>().AddChild(GetEntity());
		else
			m_Parent.Emplace<HierachyComponent>().AddChild(GetEntity());
#endif
	}

	void HierachyComponent::SetChildren(std::vector<Entity> children)
	{
		m_Children = std::move(children);

#if RAY_AUTOMATIC_CHILD_PARENT_RELATION_EMPLACEMENT
		for (Entity child : m_Children)
		{
			if (child.Has<HierachyComponent>())
				child.Get<HierachyComponent>().SetParent(GetEntity());
			else
				child.Emplace<HierachyComponent>().SetParent(GetEntity());
		}
#endif
	}

	void HierachyComponent::RemoveChild(Entity entity)
	{
		m_Children.erase(std::find(m_Children.begin(), m_Children.end(), entity));
	}

	bool HierachyComponent::HasChild(Entity child) const
	{
		return std::find(m_Children.begin(), m_Children.end(), child) != m_Children.end();
	}
}  // namespace At0::Ray
