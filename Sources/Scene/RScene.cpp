#include "Rpch.h"

#include "REntity.h"
#include "RScene.h"
#include "Utils/RException.h"

#include "Components/RMesh.h"
#include "Components/RSkybox.h"

#include "Events/REventListener.h"


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

		// Dispatch entity created event to listeners
		EntityCreatedEvent e(entity);
		for (EventListener<EntityCreatedEvent>* listener :
			EventDispatcher<EntityCreatedEvent>::Get())
		{
			listener->OnEvent(e);
		}

		return entity;
	}

	void Scene::DestroyEntity(Entity entity) { m_Registry.destroy((entt::entity)entity); }

	void Scene::Update(Delta dt)
	{
		m_Camera->Update(dt);

		m_Registry.view<Mesh>().each([&dt](Mesh& mesh) { mesh.Update(dt); });
		m_Registry.view<Skybox>().each([&dt](Skybox& skybox) { skybox.Update(dt); });
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
