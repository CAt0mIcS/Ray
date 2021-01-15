#include "../RRpch.h"
#include "RScene.h"

#include <RayUtil/RComponents.h>


namespace At0::Ray
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		// We can get all entities using the TransformComponent
		// because all entities must have one
		m_Registry.view<TransformComponent>().each([this](const auto entity, auto &&...)
			{
				m_Registry.destroy(entity);
			}
		);
	}

	void Scene::Submit(const Drawable& drawable)
	{
		m_Drawables.emplace_back(drawable);
	}

	Entity Scene::CreateEntity(std::string_view tag)
	{
		Entity e{ m_Registry.create(), this };
		e.Emplace<TransformComponent>();
		e.Emplace<TagComponent>(tag);
		return e;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}
}