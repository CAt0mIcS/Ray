#include "Rpch.h"

#include "REntity.h"
#include "RScene.h"
#include "Utils/RException.h"

#include "Components/RMesh.h"
#include "Components/RTransform.h"

#include "Events/REventListener.h"


namespace At0::Ray
{
	Scene& Scene::Get()
	{
		RAY_MEXPECTS(s_CurrentScene, "[Scene] Current Scene was not set.");
		return *s_CurrentScene;
	}

	void Scene::Set(Scope<Scene> scene) { s_CurrentScene = std::move(scene); }

	void Scene::Destroy()
	{
		if (s_CurrentScene)
			s_CurrentScene.reset();
		s_CurrentScene = nullptr;
	}

	Scene::~Scene() {}

	Entity& Scene::CreateEntity()
	{
		Scope<Entity> entity = MakeScope<Entity>(m_Registry);
		entity->Emplace<Transform>();

		// Dispatch entity created event to listeners
		EntityCreatedEvent e(*entity);
		for (EventListener<EntityCreatedEvent>* listener :
			EventDispatcher<EntityCreatedEvent>::Get())
		{
			listener->OnEvent(e);
		}

		return *m_Entities.emplace_back(std::move(entity));
	}

	void Scene::Update(Delta dt)
	{
		m_Registry.view<Mesh>().each([&dt](Mesh& mesh) { mesh.Update(dt); });
	}

	Scene::Scene() {}

}  // namespace At0::Ray
