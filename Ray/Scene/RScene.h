#pragma once

#include "../RBase.h"
#include "../Core/RTime.h"
#include "../Events/REventDispatcher.h"
#include "../Core/RResourceManager.h"
#include "../Utils/RThreadPool.h"
#include "../Events/RCustomEvents.h"
#include "../Layers/RLayer.h"

#include "REntity.h"
#include "RCamera.h"

#include <concepts>


namespace At0::Ray
{
	class DescriptorSet;
	class Camera;


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

		/**
		 * Destroys the specified entity and all it's components
		 */
		void DestroyEntity(Entity entity);

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

		const ResourceManager& GetResourceManager() const;
		ResourceManager& GetResourceManager()
		{
			return (ResourceManager&)std::as_const(*this).GetResourceManager();
		}

		template<typename T, typename... Args>
		static Scene& Create(Args&&... args)
			requires std::derived_from<T, Scene> && std::constructible_from<T, Args...>
		{
			// s_CurrentScene is set in the constructor of Scene
			new T(std::forward<Args>(args)...);
			return *s_CurrentScene;
		}

		template<typename T, typename... Args>
		T& RegisterLayer(Args&&... args)
			requires std::derived_from<T, Layer> /* && std::constructible_from<T, Scene, Args...>*/
		{
			return *(T*)(m_Layers.emplace_back(MakeScope<T>(*this, std::forward<Args>()...)).get());
		}

	protected:
		Scene(Scope<Camera> camera);

	private:
		entt::registry m_Registry;
		Scope<Camera> m_Camera = nullptr;
		ThreadPool m_ThreadPool;

		ResourceManager m_Resources;

		std::vector<Scope<Layer>> m_Layers;

		static Scope<Scene> s_CurrentScene;
	};

}  // namespace At0::Ray
