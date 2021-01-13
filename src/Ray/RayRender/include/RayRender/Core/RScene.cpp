#include "../RRpch.h"
#include "RScene.h"

#include <RayUtil/RComponents.h>


namespace At0::Ray
{
	Scene::Scene()
	{
		m_Drawables.resize(1000);
	}

	Scene::~Scene()
	{
	}

	void Scene::Submit(const Drawable& drawable)
	{
		m_Drawables.emplace_back(drawable);
	}
}