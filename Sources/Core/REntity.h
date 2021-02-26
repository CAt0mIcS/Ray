#pragma once

#include "../RBase.h"
#include "../Utils/RAssert.h"
#include "../Utils/RNonCopyable.h"

#include <../../Extern/entt/src/entt/entt.hpp>


namespace At0::Ray
{
	class Entity : NonCopyable
	{
	public:
		Entity(entt::registry& registry) : m_Registry(registry), m_EntityHandle(registry.create())
		{
		}

		/**
		 * Adds a new component to the entity
		 * @tparam Component The component to add to the entity
		 * @tparam Args Arguments to initialize the component
		 * @returns A reference to the created component
		 */
		template<typename Component, typename... Args>
		decltype(auto) Emplace(Args&&... args)
		{
			RAY_MEXPECTS(!Has<Component>(), "[Entity] Entity (ID={0}) already has component.",
				(uint32_t)m_EntityHandle);
			return m_Registry.emplace<Component>(m_EntityHandle, std::forward<Args>(args)...);
		}

		/**
		 * @tparam Components The components to check
		 * @returns If this entity has all of the specified components
		 */
		template<typename... Component>
		bool Has() const
		{
			return m_Registry.has<Component...>(m_EntityHandle);
		}

		/**
		 * @tparam Components The components to check
		 * @returns If this entity has any of the specified components
		 */
		template<typename... Component>
		bool HasAny() const
		{
			return m_Registry.any<Component...>(m_EntityHandle);
		}

		/**
		 * @tparam Component The components to get
		 * @returns The component specified or a tuple of components
		 */
		template<typename... Component>
		decltype(auto) Get()
		{
			return m_Registry.get<Component...>(m_EntityHandle);
		}

		/**
		 * @tparam Component The components to get
		 * @returns The component specified or a tuple of components
		 */
		template<typename... Component>
		decltype(auto) Get() const
		{
			return m_Registry.get<Component...>(m_EntityHandle);
		}

	private:
		entt::registry& m_Registry;
		entt::entity m_EntityHandle;
	};
}  // namespace At0::Ray
