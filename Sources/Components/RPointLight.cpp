#include "Rpch.h"
#include "RPointLight.h"

#include "RMesh.h"
#include "Scene/RScene.h"


namespace At0::Ray
{
	PointLight::PointLight(Entity entity, Float3 color, float power) : Component(entity) {}

	const Float3& PointLight::GetTranslation() const
	{
		// RAY_TODO: Light should store it's own translation and not be bound to a mesh
		return GetEntity().Get<Mesh>().GetTransform().Translation();
	}

	void PointLight::SetTranslation(Float3 translation)
	{
		// RAY_TODO: Light should store it's own translation and not be bound to a mesh
		GetEntity().Get<Mesh>().GetTransform().SetTranslation(std::move(translation));
	}
}  // namespace At0::Ray
