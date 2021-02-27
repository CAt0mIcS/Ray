#include "Rpch.h"

#include "REntity.h"
#include "RScene.h"
#include "Utils/RException.h"

#include "Components/RMesh.h"
#include "Components/RTransform.h"


namespace At0::Ray
{
	Scene::~Scene() {}

	Entity& Scene::CreateEntity()
	{
		Scope<Entity> entity = MakeScope<Entity>(m_Registry);
		entity->Emplace<Transform>();
		return *m_Entities.emplace_back(std::move(entity));
	}

	void Scene::Update(Delta dt)
	{
		auto view = m_Registry.view<Mesh>();
		view.each([&dt](Mesh& mesh) { mesh.Update(dt); });
	}

	Scene::Scene() {}

}  // namespace At0::Ray
