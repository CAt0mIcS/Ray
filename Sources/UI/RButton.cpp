#include "Rpch.h"
#include "RButton.h"

#include "Scene/RScene.h"
#include "Geometry/RMesh.h"
#include "Core/RMath.h"


namespace At0::Ray
{
	Button::Button(Entity& entity) : Widget(entity), m_PlaneEntity(Scene::Get().CreateEntity())
	{
		Material material(
			{ "Resources/Shaders/UITestShader.vert", "Resources/Shaders/UITestShader.frag" });

		Mesh& mesh = m_PlaneEntity.Emplace<Mesh>(Mesh::Plane(std::move(material)));
		Transform& transform = mesh.GetTransform();
		// transform.SetScale({ 0.2f, 0.2f, 0.2f });
		transform.SetTranslation({ -1.0f, -1.0f, 0.0f });

		Float2 screenSpaceCoords = NDCSpaceToScreenSpace({ 0.0f, 0.0f });
		Float2 ndcSpace = ScreenSpaceToNDCSpace(screenSpaceCoords);

		Log::Debug("{0}, {1}", screenSpaceCoords.x, screenSpaceCoords.y);
		Log::Debug("{0}, {1}", ndcSpace.x, ndcSpace.y);
	}
}  // namespace At0::Ray
