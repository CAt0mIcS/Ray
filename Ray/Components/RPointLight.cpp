#include "RPointLight.h"

#include "RMeshRenderingResources.h"
#include "RTransform.h"

#include "Scene/RScene.h"
#include "Shading/RMaterial.h"


namespace At0::Ray
{
	uint32_t PointLight::s_ID = 0;

	PointLight::PointLight(Entity entity, Float4 color)
		: Component(entity), EventListener(entity.Get<Transform>()), m_Color(std::move(color)),
		  m_ID(s_ID++)
	{
		RAY_MEXPECTS(m_ID < 10, "[PointLight] Light limit reached");

		// RAY_TODO: Shared material updated multiple times
		GetEntity().RegistryView<MeshRenderingResources>().each(
			[this](MeshRenderingResources& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (material.HasUniform("Shading.ptNumLights"))
					material.Set("Shading.ptNumLights", s_ID);

				if (material.HasUniform("Shading.ptLightColor" + id))
					material.Set("Shading.ptLightColor" + id, m_Color);

				if (material.HasUniform("Shading.ptLightPos" + id))
					material.Set("Shading.ptLightPos" + id,
						Float4{ GetEntity().Get<Transform>().Translation(), 1.f });
			});
	}

	void PointLight::SetColor(Float4 color)
	{
		m_Color = std::move(color);
		GetEntity().RegistryView<MeshRenderingResources>().each(
			[this](MeshRenderingResources& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (!material.HasUniform("Shading.ptLightColor" + id))
					return;

				material.Set("Shading.ptLightColor" + id, m_Color);
			});
	}

	void PointLight::SetTranslation(Float3 trans)
	{
		GetEntity().RegistryView<MeshRenderingResources>().each(
			[this, &trans](MeshRenderingResources& renderer)
			{
				std::string id = "[" + std::to_string(m_ID) + "]";

				Material& material = renderer.GetMaterial();
				if (!material.HasUniform("Shading.ptLightPos" + id))
					return;

				material.Set("Shading.ptLightPos" + id, Float4{ trans, 1.f });
			});
	}

	void PointLight::OnEvent(TransformChangedEvent& e)
	{
		SetTranslation(e.transform.Translation());
	}
}  // namespace At0::Ray
