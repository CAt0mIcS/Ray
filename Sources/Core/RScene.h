#pragma once

#include "../RBase.h"
#include "REntity.h"
#include "RTime.h"

#include "../Events/REventDispatcher.h"


namespace At0::Ray
{
	/**
	 * Gets dispatched to listeners when entity is added to the scene
	 */
	class EntityCreatedEvent
	{
	public:
		EntityCreatedEvent(Entity& entity) : m_Entity(entity) {}
		Entity& GetEntity() const { return m_Entity; }

	private:
		Entity& m_Entity;
	};


	class RAY_EXPORT Scene : public EventDispatcher<EntityCreatedEvent>
	{
	public:
		static Scene& Get()
		{
			static Scene instance;
			return instance;
		}

		virtual ~Scene();

		/**
		 * @returns A newly created, empty entity
		 */
		Entity& CreateEntity();

		/**
		 * Creates iterable list of all components of the Comp type
		 */
		template<typename... Comp>
		auto EntityView()
		{
			return m_Registry.view<Comp...>();
		}

		/**
		 * Updates all components
		 */
		void Update(Delta dt);

	protected:
		Scene();

	private:
		entt::registry m_Registry;

		// Heap allocate them to avoid Entity& from being invalid when the vector is resized
		std::vector<Scope<Entity>> m_Entities;
	};

}  // namespace At0::Ray
