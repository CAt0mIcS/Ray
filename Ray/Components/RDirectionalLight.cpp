#include "RDirectionalLight.h"
#include "RMeshRenderer.h"
#include "RTransform.h"

#include "Scene/RScene.h"
#include "Shading/RMaterial.h"


namespace At0::Ray
{
	uint32_t DirectionalLight::s_ID = 0;

	DirectionalLight::DirectionalLight(Entity entity, Float4 color)
		: Component(entity), m_ID(s_ID++), m_Color(std::move(color))
	{
		RAY_MEXPECTS(m_ID < 10, "[DirectionalLight] Light limit reached");

		// RAY_TODO: Shared material updated multiple times
		Scene::Get().EntityView<MeshRenderer>().each(
			[this](MeshRenderer& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (material.HasUniform("Shading.dirNumLights"))
					material.Set("Shading.dirNumLights", s_ID);

				if (material.HasUniform("Shading.dirLightColor" + id))
					material.Set("Shading.dirLightColor" + id, m_Color);

				if (material.HasUniform("Shading.dirLightPos" + id))
					material.Set("Shading.dirLightPos" + id,
						Float4{ GetEntity().Get<Transform>().Translation(), 1.f });
			});

		// Apply default up direction
		SetDirection(m_Direction);
	}

	void DirectionalLight::SetColor(Float4 color)
	{
		m_Color = std::move(color);
		Scene::Get().EntityView<MeshRenderer>().each(
			[this](MeshRenderer& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (!material.HasUniform("Shading.dirLightColor" + id))
					return;

				material.Set("Shading.dirLightColor" + id, m_Color);
			});
	}

	void DirectionalLight::SetDirection(Float3 dir)
	{
		m_Direction = std::move(dir);
		Scene::Get().EntityView<MeshRenderer>().each(
			[this](MeshRenderer& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (!material.HasUniform("Shading.dirLightDirection" + id))
					return;

				material.Set("Shading.dirLightDirection" + id, Float4{ m_Direction, 1.f });
			});
	}
}  // namespace At0::Ray