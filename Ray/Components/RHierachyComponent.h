#pragma once

#include "RComponent.h"


namespace At0::Ray
{
	class RAY_EXPORT HierachyComponent : public Component
	{
	public:
		HierachyComponent(Entity entity);

		void AddChild(Entity entity);
		void AddChildren(const std::vector<Entity>& children);
		void SetParent(Entity parent);
		void SetChildren(std::vector<Entity> children);

		void RemoveChild(Entity entity);

		bool HasChild(Entity child) const;

		const std::vector<Entity>& GetChildren() const { return m_Children; }
		std::vector<Entity>& GetChildren() { return m_Children; }
		Entity GetParent() const { return m_Parent; }

	private:
		std::vector<Entity> m_Children;
		Entity m_Parent = Entity::Null;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(HierachyComponent);
