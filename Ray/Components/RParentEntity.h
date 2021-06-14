#pragma once
#pragma once

#include "RComponent.h"


namespace At0::Ray
{
	/**
	 * Add to child entity to define a parent (RAY_TODO)
	 */
	class RAY_EXPORT ParentEntity : public Component
	{
	public:
		ParentEntity(Entity entity, Entity parent);

		Entity GetParent() const { return m_Parent; }
		void SetParent(Entity entity) { m_Parent = entity; }

	private:
		Entity m_Parent;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(ParentEntity);
