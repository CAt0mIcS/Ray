
#include "REntity.h"
#include "RScene.h"

#include "Components/RMeshRenderer.h"
#include "Components/RSkybox.h"
#include "Components/RTransform.h"

#include "Events/REventListener.h"

#define RAY_MULTITHREADED_TRANSFORM_CALCULATIONS 1
#define RAY_MULTITHREADED_MESHRENDERER_UPDATES 1


namespace At0::Ray
{
	Scope<Scene> Scene::s_CurrentScene = nullptr;

	Scene& Scene::Get()
	{
		RAY_MEXPECTS(s_CurrentScene, "[Scene] Current Scene was not set");
		return *s_CurrentScene;
	}

	void Scene::Destroy() { s_CurrentScene = nullptr; }

	Scene::~Scene() { m_Registry.clear(); }

	Entity Scene::CreateEntity()
	{
		Entity entity(m_Registry.create(), &m_Registry);

		// RAY_TODO: Should maybe be moved to client
		entity.Emplace<Transform>();

		// Dispatch entity created event to listeners
		EntityCreatedEvent e(entity);
		for (EventListener<EntityCreatedEvent>* listener :
			EventDispatcher<EntityCreatedEvent>::Get())
		{
			listener->OnEvent(e);
		}

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		if (entity.HasParent())
			for (Entity child : entity.GetChildren())
				DestroyEntity(child);

		m_Registry.destroy((entt::entity)entity);
	}

	void Scene::Update(Delta dt)
	{
		m_Camera->Update(dt);

		auto tformView = m_Registry.view<Transform>();

		// RAY_TODO: Global define to enable/disable profiling
		Time tStart = Time::Now();

#if RAY_MULTITHREADED_TRANSFORM_CALCULATIONS
		m_ThreadPool.SubmitLoop(0u, (uint32_t)tformView.size(),
			[this, &tformView](uint32_t i)
			{ Entity{ tformView[i] }.Get<Transform>().UpdateMatrix(); });
		m_ThreadPool.WaitForTasks();
#else
		tformView.each([](Transform& tform) { tform.UpdateMatrix(); });
#endif

		// CLog::Trace("[Scene] Transformation recalculations took {0}us",
		//	(Time::Now() - tStart).AsMicroseconds());

		tStart = Time::Now();

#if RAY_MULTITHREADED_MESHRENDERER_UPDATES
		auto meshRendererView = m_Registry.view<MeshRenderer>();
		m_ThreadPool.SubmitLoop(0u, (uint32_t)meshRendererView.size(),
			[&meshRendererView](uint32_t i)
			{ Entity{ meshRendererView[i] }.Get<MeshRenderer>().Update(); });
		m_ThreadPool.WaitForTasks();

#else
		m_Registry.view<MeshRenderer>().each([](MeshRenderer& mesh) { mesh.Update(); });
#endif

		// CLog::Trace(
		//	"[Scene] MeshRenderer updates took {0}us", (Time::Now() - tStart).AsMicroseconds());
	}

	void Scene::SetCamera(Scope<Camera> cam) { m_Camera = std::move(cam); }

	const Camera& Scene::GetCamera() const
	{
		RAY_MEXPECTS(m_Camera, "[Scene] Camera not created");
		return *m_Camera;
	}

	Scene::Scene(Scope<Camera> camera) : m_Camera(std::move(camera))
	{
		s_CurrentScene = Scope<Scene>(this);
	}
}  // namespace At0::Ray
