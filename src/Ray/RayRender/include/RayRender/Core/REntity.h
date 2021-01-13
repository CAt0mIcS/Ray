#pragma once

#include "../RRBase.h"

#include <../extern/entt/src/entt/entt.hpp>


namespace At0::Ray
{
	class RR_API Entity
	{
	public:
		Entity();
		~Entity();

		/// <summary>
		/// Adds any component to the entity and initializes it with the arguments
		/// </summary>
		/// <returns>Reference to the added component or void if the component is empty</returns>
		template<typename Component, typename... Args>
		decltype(auto) Emplace(Args&&... args)
		{
			assert(!Has<Component>() && "[Entity::Remove] Entity already has the specified component.");
			s_Registry.emplace<Component>(m_Entity, std::forward<Args>(args)...);
		}

		/// <summary>
		/// Removes the specified component. Function will assert false if 
		/// the component does not exist.
		/// </summary>
		template<typename Component>
		void Remove()
		{
			assert(Has<Component>() && "[Entity::Remove] Entity does not have component.");
			s_Registry.remove<Component>(m_Entity);
		}

		/// <summary>
		/// Checks if the entity already has the component
		/// </summary>
		template<typename Component>
		bool Has() const
		{
			return s_Registry.has<Component>(m_Entity);
		}

	private:
		entt::entity m_Entity;
		inline static entt::registry s_Registry;
	};
}