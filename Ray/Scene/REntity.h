﻿#pragma once

#include "../RBase.h"
#include "../Utils/RAssert.h"

#include <../../Extern/entt/src/entt/entt.hpp>


namespace At0::Ray
{
	class RAY_EXPORT Entity
	{
	public:
		static constexpr entt::entity Null = (entt::entity)-1;

	public:
		Entity() = default;

		/**
		 * Construct an entity from an existing entitty handle
		 * @param handle Entity handle
		 * @param registry Registry from which the entity was created
		 */
		Entity(entt::entity handle, entt::registry* registry);

		/**
		 * Construct an entity from an existing entity handle. Entity must've been created from the
		 * Scene's registry (default)
		 */
		Entity(entt::entity handle);

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
			RAY_MEXPECTS(Valid(), "[Entity] Cannot assign component to null entity.");

			if constexpr (std::is_constructible_v<Comp, Entity&, Args...>)
				return m_Registry->emplace<Comp>(
					m_EntityHandle, *this, std::forward<Args>(args)...);
			else
				return m_Registry->emplace<Comp>(m_EntityHandle, std::forward<Args>(args)...);
		}

		/**
		 * If the entity already has the component it will just be returned
		 * Otherwise it will be added and returned
		 */
		template<typename Comp, typename... Args>
		decltype(auto) GetOrEmplace(Args&&... args)
		{
			RAY_MEXPECTS(Valid(), "[Entity] Cannot assign component to null entity.");

			if constexpr (std::is_constructible_v<Comp, Entity&, Args...>)
				return m_Registry->get_or_emplace<Comp>(
					m_EntityHandle, *this, std::forward<Args>(args)...);
			else
				return m_Registry->get_or_emplace<Comp>(m_EntityHandle, std::forward<Args>()...);
		}

		/**
		 * @returns Component if the entity has it, otherwise nullptr
		 */
		template<typename... Comp>
		decltype(auto) TryGet() const
		{
			RAY_MEXPECTS(Valid(), "[Entity] Cannot assign component to null entity.");
			return m_Registry->try_get<Comp...>(m_EntityHandle);
		}

		/**
		 * @returns Component if the entity has it, otherwise nullptr
		 */
		template<typename... Comp>
		decltype(auto) TryGet()
		{
			RAY_MEXPECTS(Valid(), "[Entity] Cannot assign component to null entity.");
			return m_Registry->try_get<Comp...>(m_EntityHandle);
		}

		/**
		 * @tparam Components The components to check
		 * @returns If this entity has all of the specified components
		 */
		template<typename... Comp>
		bool Has() const
		{
			RAY_MEXPECTS(Valid(), "[Entity] Cannot check component(s) of null entity.");
			return m_Registry->has<Comp...>(m_EntityHandle);
		}

		/**
		 * @tparam Components The components to check
		 * @returns If this entity has any of the specified components
		 */
		template<typename... Comp>
		bool HasAny() const
		{
			RAY_MEXPECTS(Valid(), "[Entity] Cannot check component(s) of null entity.");
			return m_Registry->any<Comp...>(m_EntityHandle);
		}

		/**
		 * @tparam Component The components to get
		 * @returns The component specified or a tuple of components
		 */
		template<typename... Comp>
		decltype(auto) Get()
		{
			RAY_MEXPECTS(Valid(), "[Entity] Cannot get component(s) of null entity.");
			RAY_MEXPECTS(Has<Comp...>(), "[Entity] Does not have specified component(s)");
			return m_Registry->get<Comp...>(m_EntityHandle);
		}

		/**
		 * @tparam Component The components to get
		 * @returns The component specified or a tuple of components
		 */
		template<typename... Comp>
		decltype(auto) Get() const
		{
			RAY_MEXPECTS(Valid(), "[Entity] Cannot get component(s) of null entity.");
			RAY_MEXPECTS(Has<Comp...>(), "[Entity] Does not have specified component(s)");
			return m_Registry->get<Comp...>(m_EntityHandle);
		}

		/**
		 * Destroys the entity and all of its components
		 */
		void Destroy();

		/**
		 * @returns True if the entity is not the null entity
		 */
		constexpr bool Valid() const { return m_EntityHandle != Entity::Null; }

		/**
		 * Adds/Removes/Changes the ParentEntity component's entity
		 */
		void SetParent(Entity parent);

		/**
		 * Adds a child to the entity
		 */
		void AddChild(Entity child);

		/**
		 * Adds list of children to the entity
		 */
		void AddChildren(const std::vector<Entity>& children);

		/**
		 * Checks if the entity has the ParentEntity component
		 */
		bool HasParent() const;

		/**
		 * @returns Parent entity, fails if the parent does not exist
		 */
		Entity GetParent() const;

		/**
		 * @returns Children of the entity, fails if it doesn't have a HierachyComponent
		 */
		const std::vector<Entity>& GetChildren() const;

		/**
		 * @returns if this has child
		 */
		bool HasChild(Entity child) const;

		/**
		 * Removes child if it exists
		 */
		void RemoveChild(Entity child);

		/**
		 * Casting operator to the entity identifier
		 */
		explicit operator entt::entity() const { return m_EntityHandle; }
		explicit operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		constexpr bool operator==(const Entity& other) const;
		constexpr bool operator!=(const Entity& other) const;

		constexpr operator bool() { return Valid(); }

		RAY_EXPORT friend std::ostream& operator<<(std::ostream& os, Entity e);

	private:
		entt::registry* m_Registry = nullptr;
		entt::entity m_EntityHandle = Null;
	};
}  // namespace At0::Ray
