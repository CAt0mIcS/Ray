#pragma once

#include "Ray/RBase.h"
#include "RayBase/RAssert.h"
#include "Ray/Scene/REntity.h"


namespace At0::Ray
{
	/**
	 * Base class for all components
	 */
	class Component
	{
	public:
		virtual ~Component() = default;

		const Entity GetEntity() const { return m_Entity; }
		Entity GetEntity() { return m_Entity; }

		Component& operator=(Component&& other) noexcept
		{
			m_Entity = other.m_Entity;
			return *this;
		}
		Component(Component&& other) noexcept { *this = std::move(other); }

	protected:
		Component(Entity entity) : m_Entity(entity) {}
		void SetEntity(Entity e) { m_Entity = e; }

	protected:
		Entity m_Entity;
	};
}  // namespace At0::Ray
