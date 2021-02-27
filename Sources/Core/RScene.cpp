#include "Rpch.h"

#include "REntity.h"
#include "RScene.h"
#include "Utils/RException.h"

#include "Components/RMesh.h"
#include "Components/RModel.h"
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
		for (Scope<Entity>& entity : m_Entities)
		{
			// if (entity->Has<Mesh>())
			//	entity->Get<Mesh>().Update(dt);
			if (entity->Has<Model>())
				entity->Get<Model>().Update(dt);
		}

		// Does not work for some reason!
		// auto meshView = m_Registry.view<Mesh>();
		// meshView.each([&dt](Mesh& mesh) { mesh.Update(dt); });

		// auto modelView = m_Registry.view<Model>();
		// modelView.each([&dt](Model& model) { model.Update(dt); });
	}

	Scene::Scene() {}

}  // namespace At0::Ray
