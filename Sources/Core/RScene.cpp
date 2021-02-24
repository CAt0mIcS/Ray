#include "Rpch.h"

#include "REntity.h"
#include "RScene.h"
#include "Utils/RException.h"


namespace At0::Ray
{
	Scene::~Scene() {}

	Entity Scene::CreateEntity() { return { m_Registry }; }

	Scene::Scene() {}

}  // namespace At0::Ray
