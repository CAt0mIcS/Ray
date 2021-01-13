#include "../RRpch.h"
#include "RScene.h"


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
		m_Drawables.push_back(std::move(drawable));
	}
}