#include "RSpotLight.h"
#include "RMeshRenderingResources.h"
#include "RTransform.h"

#include "Scene/RScene.h"
#include "Shading/RMaterial.h"


namespace At0::Ray
{
	uint32_t SpotLight::s_ID = 0;

	SpotLight::SpotLight(Entity entity, Float4 color)
		: Component(entity), m_ID(s_ID++), m_Color(std::move(color))
	{
		RAY_MEXPECTS(m_ID < 10, "[SpotLight] Light limit reached");

		// RAY_TODO: Shared material updated multiple times
		GetEntity().RegistryView<MeshRenderingResources>().each(
			[this](MeshRenderingResources& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (material.HasUniform("Shading.spotNumLights"))
					material.Set("Shading.spotNumLights", s_ID);

				if (material.HasUniform("Shading.spotLightColor" + id))
					material.Set("Shading.spotLightColor" + id, m_Color);

				if (material.HasUniform("Shading.spotLightPos" + id))
					material.Set("Shading.spotLightPos" + id,
						Float4{ GetEntity().Get<Transform>().Translation(), 1.f });
			});

		// Apply default down direction and angle
		SetDirection(m_Direction);
		SetAngle(m_AngleRadians);
	}

	void SpotLight::SetColor(Float4 color)
	{
		m_Color = std::move(color);
		GetEntity().RegistryView<MeshRenderingResources>().each(
			[this](MeshRenderingResources& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (!material.HasUniform("Shading.spotLightColor" + id))
					return;

				material.Set("Shading.spotLightColor" + id, m_Color);
			});
	}

	void SpotLight::SetTranslation(Float3 trans)
	{
		const auto& tform = GetEntity().Get<Transform>().SetTranslation(trans);

		GetEntity().RegistryView<MeshRenderingResources>().each(
			[this, &tform](MeshRenderingResources& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (!material.HasUniform("Shading.spotLightPos" + id))
					return;

				material.Set("Shading.spotLightPos" + id, Float4{ tform.Translation(), 1.f });
			});
	}

	void SpotLight::SetAngle(float angleRadians)
	{
		m_AngleRadians = angleRadians;
		GetEntity().RegistryView<MeshRenderingResources>().each(
			[this](MeshRenderingResources& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (!material.HasUniform("Shading.spotAngle" + id))
					return;

				material.Set("Shading.spotAngle" + id, cos(m_AngleRadians));
			});
	}

	void SpotLight::SetDirection(Float3 dir)
	{
		m_Direction = std::move(dir);
		GetEntity().RegistryView<MeshRenderingResources>().each(
			[this](MeshRenderingResources& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (!material.HasUniform("Shading.spotLightDirection" + id))
					return;

				material.Set("Shading.spotLightDirection" + id, Float4{ m_Direction, 1.f });
			});
	}
}  // namespace At0::Ray