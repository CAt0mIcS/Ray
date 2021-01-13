#include "../RRpch.h"
#include "RScene.h"


namespace At0::Ray
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Submit(Drawable&& drawable)
	{
		m_Drawables.emplace_back(std::move(drawable));
	}
}