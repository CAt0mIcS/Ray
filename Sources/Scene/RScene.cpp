#include "Rpch.h"

#include "REntity.h"
#include "RScene.h"
#include "Utils/RException.h"

#include "Components/RMesh.h"
#include "Components/RModel.h"

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

	Scene::~Scene()
	{
		for (Entity& entity : m_Entities)
		{
			m_Registry.destroy((entt::entity)entity);
		}
	}

	Entity& Scene::CreateEntity()
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
		auto meshView = m_Registry.view<Mesh>();
		meshView.each([&dt](Mesh& mesh) { mesh.Update(dt); });

		auto modelView = m_Registry.view<Model>();
		modelView.each([&dt](Model& model) { model.Update(dt); });
	}

	Scene::Scene() {}

}  // namespace At0::Ray
