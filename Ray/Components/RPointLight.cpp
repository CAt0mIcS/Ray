#include "RPointLight.h"

#include "RMeshRenderer.h"
#include "RTransform.h"

#include "Scene/RScene.h"
#include "Shading/RMaterial.h"


namespace At0::Ray
{
	uint32_t PointLight::s_ID = 0;

	PointLight::PointLight(Entity entity, Float4 color)
		: Component(entity), m_Color(std::move(color)), m_ID(s_ID++)
	{
		RAY_MEXPECTS(m_ID < 10, "[PointLight] Light limit reached");

		// RAY_TODO: Shared material updated multiple times
		Scene::Get().EntityView<MeshRenderer>().each(
			[this](MeshRenderer& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (!material.HasUniform("Shading.numLights") ||
					!material.HasUniform("Shading.lightColor" + id) ||
					!material.HasUniform("Shading.lightPosition" + id))
					return;

				material.Set("Shading.numLights", s_ID);

				material.Set("Shading.lightColor" + id, m_Color);
				material.Set("Shading.lightPosition" + id,
					Float4{ GetEntity().Get<Transform>().Translation(), 1.f });
			});
	}

	void PointLight::SetColor(Float4 color)
	{
		m_Color = color;
		Scene::Get().EntityView<MeshRenderer>().each(
			[this](MeshRenderer& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (!material.HasUniform("Shading.lightColor" + id))
					return;

				material.Set("Shading.lightColor" + id, m_Color);
			});
	}

	void PointLight::SetTranslation(Float3 trans)
	{
		const Transform& tform = GetEntity().Get<Transform>().SetTranslation(std::move(trans));

		Scene::Get().EntityView<MeshRenderer>().each(
			[this, &tform](MeshRenderer& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (!material.HasUniform("Shading.lightPosition" + id))
					return;

				material.Set("Shading.lightPosition" + id, Float4{ tform.Translation(), 1.f });
			});
	}
}  // namespace At0::Ray
