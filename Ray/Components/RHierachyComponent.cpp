#include "Rpch.h"
#include "RHierachyComponent.h"


namespace At0::Ray
{
	HierachyComponent::HierachyComponent(Entity entity) : Component(entity) {}

	void HierachyComponent::AddChild(Entity child)
	{
		RAY_MEXPECTS(!HasChild(child),
			"[HierachyComponent] Entity {0} already has entity {1} as child", GetEntity(), child);
		m_Children.emplace_back(child);
	}

	bool HierachyComponent::HasChild(Entity child) const
	{
		return std::find(m_Children.begin(), m_Children.end(), child) != m_Children.end();
	}
}  // namespace At0::Ray