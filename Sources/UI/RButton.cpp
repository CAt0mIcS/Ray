#include "Rpch.h"
#include "RButton.h"

#include "Scene/RScene.h"
#include "Geometry/RMesh.h"
#include "Core/RMath.h"


namespace At0::Ray
{
	Button::Button(Entity& entity, Float2 pos)
		: Widget(entity), m_PlaneEntity(Scene::Get().CreateEntity())
	{
		Material material(
			{ "Resources/Shaders/UITestShader.vert", "Resources/Shaders/UITestShader.frag" });

		Mesh& mesh = m_PlaneEntity.Emplace<Mesh>(Mesh::Plane(std::move(material)));
		Transform& transform = mesh.GetTransform();
		// transform.SetScale({ 0.2f, 0.2f, 0.2f });
		transform.SetTranslation({ ScreenSpaceToNDCSpace(pos) + 0.5f, 0.0f });
	}
}  // namespace At0::Ray
