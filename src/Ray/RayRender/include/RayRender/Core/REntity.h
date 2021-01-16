#pragma once

#include "../RRBase.h"

#include <../../../extern/entt/src/entt/entt.hpp>


namespace At0::Ray
{
	class RR_API Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, entt::registry* registry);
		Entity(const Entity& other) = default;

		/// <summary>
		/// Adds any component to the entity and initializes it with the arguments
		/// </summary>
		/// <returns>Reference to the added component or void if the component is empty</returns>
		template<typename Component, typename... Args>
		decltype(auto) Emplace(Args&&... args)
		{
			assert(!Has<Component>() && "[Entity::Emplace] Entity already has the specified component.");
			return m_Registry->emplace<Component>(m_Entity, std::forward<Args>(args)...);
		}

		/// <summary>
		/// Removes the specified component. Function will assert false if 
		/// the component does not exist.
		/// </summary>
		template<typename... Component>
		void Remove()
		{
			assert(Has<Component...>() && "[Entity::Remove] Entity does not have component.");
			m_Registry->remove<Component...>(m_Entity);
		}

		/// <summary>
		/// Checks if the entity already has the component
		/// </summary>
		template<typename... Component>
		bool Has() const
		{
			return m_Registry->has<Component...>(m_Entity);
		}

		/// <summary>
		/// Gets any number of components from the entity
		/// </summary>
		template<typename... Component>
		decltype(auto) Get()
		{
			assert(Has<Component...>() && "[Entity::Get] Entity does not have component.");
			return m_Registry->get<Component...>(m_Entity);
		}

		operator bool() const { return m_Entity != entt::null; }
		operator entt::entity() const { return m_Entity; }

	private:
		entt::entity m_Entity;
		entt::registry* m_Registry;
	};
}
