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

		const Entity& GetEntity() const { return *m_Entity; }
		Entity& GetEntity() { return const_cast<Entity&>(std::as_const(*this).GetEntity()); }

		bool EntitySet() const { return m_Entity != nullptr; }

		Component& operator=(Component&& other) { m_Entity = other.m_Entity; }
		Component(Component&& other) { *this = std::move(other); }

	protected:
		Component(Entity& entity) : m_Entity(&entity) {}
		void SetEntity(Entity& e) { m_Entity = &e; }

	protected:
		Entity* m_Entity = nullptr;
	};
}  // namespace At0::Ray
