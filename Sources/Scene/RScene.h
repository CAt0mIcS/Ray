#pragma once

#include "../RBase.h"
#include "REntity.h"
#include "../Core/RTime.h"
#include "RCamera.h"

#include "../Events/REventDispatcher.h"

#include <concepts>


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
		static Scene& Get();
		static void Destroy();

		virtual ~Scene();

		/**
		 * @returns A newly created, empty entity
		 */
		Entity CreateEntity();

		const auto& GetEntities() const { return m_Entities; }

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

		/**
		 * @returns The entity storage registry
		 */
		entt::registry& GetRegistry() { return m_Registry; }

		void SetCamera(Scope<Camera> camera);
		const Camera& GetCamera() const;
		Camera& GetCamera() { return (Camera&)std::as_const(*this).GetCamera(); }

		template<typename T, typename... Args>
		static Scene& Create(Args&&... args) requires std::derived_from<T,
			Scene>&& std::constructible_from<T, Args...>
		{
			// s_CurrentScene is set in the constructor of Scene
			new T(std::forward<Args>(args)...);
			return *s_CurrentScene;
		}

	protected:
		Scene(Scope<Camera> camera);

	private:
		entt::registry m_Registry;
		Scope<Camera> m_Camera = nullptr;

		// Heap allocate them to avoid Entity& from being invalid when the vector is resized
		std::vector<Entity> m_Entities;
		static Scope<Scene> s_CurrentScene;
	};

}  // namespace At0::Ray
