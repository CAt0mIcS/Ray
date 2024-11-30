#pragma once

#include "../RBase.h"
#include "../Core/RTime.h"
#include "../Events/REventDispatcher.h"
#include "../Core/RResourceManager.h"
#include "../Utils/RThreadPool.h"
#include "../Events/RCustomEvents.h"

#include "REntity.h"
#include "RCamera.h"

#include <concepts>


namespace At0::Ray
{
	class DescriptorSet;
	class Camera;
	class Layer;
	class Engine;
	class Window;


	class RAY_EXPORT Scene : public EventDispatcher<EntityCreatedEvent>, public Resource
	{
		friend class Engine;

	public:
		Scene(Engine& engine, Scope<Camera> camera);
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
		 * Updates transforms as well as uniform buffers for cameras etc.
		 */
		void UpdateTransforms(Delta dt);

		/**
		 * @returns The entity storage registry
		 */
		entt::registry& GetRegistry() { return m_Registry; }

		void SetCamera(Scope<Camera> camera);
		const Camera& GetCamera() const;
		Camera& GetCamera() { return (Camera&)std::as_const(*this).GetCamera(); }

		const Window& GetWindow() const;
		Window& GetWindow() { return (Window&)std::as_const(*this).GetWindow(); }

		const Engine& GetEngine() const { return m_Engine; }

		template<typename T, typename... Args>
		T& RegisterLayer(Args&&... args)
			requires std::derived_from<T, Layer> /* && std::constructible_from<T, Scene, Args...>*/
		{
			return *(T*)(m_Layers.emplace_back(MakeScope<T>(*this, std::forward<Args>()...)).get());
		}

	private:
		void SetWindow(Ref<Window> window);

	private:
		entt::registry m_Registry;
		Scope<Camera> m_Camera = nullptr;
		ThreadPool m_ThreadPool;

		std::vector<Scope<Layer>> m_Layers;
		const Engine& m_Engine;
		WeakPtr<Window> m_Window;
	};

}  // namespace At0::Ray
