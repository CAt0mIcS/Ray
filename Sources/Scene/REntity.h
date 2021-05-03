#pragma once

#include "../RBase.h"
#include "../Utils/RAssert.h"
#include "../Utils/RNonCopyable.h"

#include <../../Extern/entt/src/entt/entt.hpp>


namespace At0::Ray
{
	class Entity
	{
	public:
		Entity() = default;

		Entity(entt::entity handle, entt::registry* registry)
			: m_EntityHandle(handle), m_Registry(registry)
		{
		}

		Entity(const Entity& other) = default;

		/**
		 * Adds a new component to the entity
		 * @tparam Component The component to add to the entity
		 * @tparam Args Arguments to initialize the component
		 * @returns A reference to the created component
		 */
		template<typename Comp, typename... Args>
		decltype(auto) Emplace(Args&&... args)
		{
			RAY_MEXPECTS(!Has<Comp>(), "[Entity] Entity (ID={0}) already has component",
				(uint32_t)m_EntityHandle);
			if constexpr (std::is_constructible_v<Comp, Entity&, Args...>)
			{
				return m_Registry->emplace<Comp>(
					m_EntityHandle, *this, std::forward<Args>(args)...);
			}
			else
			{
				return m_Registry->emplace<Comp>(m_EntityHandle, std::forward<Args>(args)...);
			}
		}

		/**
		 * @tparam Components The components to check
		 * @returns If this entity has all of the specified components
		 */
		template<typename... Comp>
		bool Has() const
		{
			return m_Registry->has<Comp...>(m_EntityHandle);
		}

		/**
		 * @tparam Components The components to check
		 * @returns If this entity has any of the specified components
		 */
		template<typename... Comp>
		bool HasAny() const
		{
			return m_Registry->any<Comp...>(m_EntityHandle);
		}

		/**
		 * @tparam Component The components to get
		 * @returns The component specified or a tuple of components
		 */
		template<typename... Comp>
		decltype(auto) Get()
		{
			return m_Registry->get<Comp...>(m_EntityHandle);
		}

		/**
		 * @tparam Component The components to get
		 * @returns The component specified or a tuple of components
		 */
		template<typename... Comp>
		decltype(auto) Get() const
		{
			return m_Registry->get<Comp...>(m_EntityHandle);
		}

		/**
		 * Casting operator to the entity identifier
		 */
		explicit operator entt::entity() const { return m_EntityHandle; }

	private:
		entt::registry* m_Registry;
		entt::entity m_EntityHandle;
	};
}  // namespace At0::Ray
