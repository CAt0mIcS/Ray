#pragma once

#include "Ray/RBase.h"
#include "../Core/RTime.h"
#include "Ray/Core/RMath.h"
#include "Ray/Components/RTransform.h"
#include "Ray/Graphics/Core/RSharedBindable.h"
#include "Ray/Events/REventDispatcher.h"
#include "../Utils/RThreadPool.h"
#include "REntity.h"
#include "RCamera.h"

#include <concepts>

typedef struct RrDescriptorSetLayout_T* RrDescriptorSetLayout;
typedef struct RrPipelineLayout_T* RrPipelineLayout;
typedef struct RrDescriptorPool_T* RrDescriptorPool;

namespace At0::Ray
{
	class DescriptorSet;
	class BufferUniform;

	struct PerSceneData
	{
		Matrix View = MatrixIdentity();
		Matrix Projection = MatrixIdentity();
		Float3 ViewPos{};
		Float3 LightPos{};
	};

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

	class RAY_EXPORT Scene :
		public EventDispatcher<EntityCreatedEvent>,
		EventListener<CameraChangedEvent>
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

		template<typename T, typename... Args>
		static Scene& Create(Args&&... args) requires std::derived_from<T,
			Scene>&& std::constructible_from<T, Args...>
		{
			// s_CurrentScene is set in the constructor of Scene
			new T(std::forward<Args>(args)...);
			return *s_CurrentScene;
		}

		// const DescriptorSet& GetPerSceneDescriptor() const { return *m_PerSceneDescriptor; }

		void CmdBind(const CommandBuffer& cmdBuff) const;

	protected:
		Scene(Scope<Camera> camera);

	private:
		void OnEvent(CameraChangedEvent& e) override { UpdateUniform(); }
		void SetupPerSceneUniform();
		void UpdateUniform();

	private:
		entt::registry m_Registry;
		Scope<Camera> m_Camera = nullptr;
		ThreadPool m_ThreadPool;

		RrDescriptorSetLayout m_DescriptorSetLayout;
		RrPipelineLayout m_PipelineLayout;
		RrDescriptorPool m_DescriptorPool;
		Scope<DescriptorSet> m_PerSceneDescriptor;
		Scope<BufferUniform> m_PerSceneUniform;

		static Scope<Scene> s_CurrentScene;
	};

}  // namespace At0::Ray
