#include "Rpch.h"

#include "REntity.h"
#include "RScene.h"
#include "Utils/RException.h"

#include "Geometry/RMesh.h"

#include "Events/REventListener.h"


namespace At0::Ray
{
	Scope<Scene> Scene::s_CurrentScene = nullptr;

	Scene& Scene::Get()
	{
		RAY_MEXPECTS(s_CurrentScene, "[Scene] Current Scene was not set");
		return *s_CurrentScene;
	}

	void Scene::Destroy()
	{
		if (s_CurrentScene)
			s_CurrentScene.reset();
		s_CurrentScene = nullptr;
	}

	Scene::~Scene()
	{
		for (Entity& entity : m_Entities)
		{
			m_Registry.destroy((entt::entity)entity);
		}
	}

	Entity Scene::CreateEntity()
	{
		Entity entity(m_Registry.create(), &m_Registry);

		// Dispatch entity created event to listeners
		EntityCreatedEvent e(entity);
		for (EventListener<EntityCreatedEvent>* listener :
			EventDispatcher<EntityCreatedEvent>::Get())
		{
			listener->OnEvent(e);
		}

		return m_Entities.emplace_back(std::move(entity));
	}

	void Scene::Update(Delta dt)
	{
		m_Camera->Update(dt);

		auto meshView = m_Registry.view<Mesh>();
		meshView.each([&dt](Mesh& mesh) { mesh.Update(dt); });
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
