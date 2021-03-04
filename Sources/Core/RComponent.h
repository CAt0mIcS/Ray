#pragma once

#include "../RBase.h"
#include "../Utils/RAssert.h"


namespace At0::Ray
{

	class Entity;

	/**
	 * Base class for all components
	 */
	class Component
	{
	public:
		virtual ~Component() = default;

		void SetEntity(Entity& e) { m_Entity = &e; }

		const Entity& GetEntity() const
		{
			RAY_MEXPECTS(m_Entity, "[Component] Entity was not set.");
			return *m_Entity;
		}

		Entity& GetEntity() { return const_cast<Entity&>(std::as_const(*this).GetEntity()); }
		bool EntitySet() const { return m_Entity != nullptr; }

	protected:
		Component() = default;

	private:
		// Set when emplacing the component
		Entity* m_Entity = nullptr;
	};
}  // namespace At0::Ray
