#include "Gizmo.h"

#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Graphics/Pipelines/Shader/RShader.h>

#include <Ray/Shading/RMaterial.h>

#include <Ray/Components/RMesh.h>
#include <Ray/Components/RMeshRenderer.h>


namespace At0::Raditor
{
	Gizmo& Gizmo::Get()
	{
		static Gizmo gizmo;
		return gizmo;
	}

	Gizmo::Gizmo()
		: m_TransEntity({ Ray::Scene::Get().CreateEntity(), Ray::Scene::Get().CreateEntity(),
			  Ray::Scene::Get().CreateEntity() }),
		  m_ScaleEntity({ Ray::Scene::Get().CreateEntity(), Ray::Scene::Get().CreateEntity(),
			  Ray::Scene::Get().CreateEntity() }),
		  m_RotEntity({ Ray::Scene::Get().CreateEntity(), Ray::Scene::Get().CreateEntity(),
			  Ray::Scene::Get().CreateEntity() })
	{
		auto pipeline =
			Ray::GraphicsPipeline::Builder()
				.SetShader(Ray::Shader::FromSourceFile(
					{ "Resources/Shaders/Flat_Col.vert", "Resources/Shaders/Flat_Col.frag" }))
				.Acquire();

		auto redMaterial = Ray::Material::Builder(pipeline)
							   .Set("Shading.color", Ray::Float4{ 1.f, 0.f, 0.f, 1.f })
							   .Acquire();

		auto greenMaterial = Ray::Material::Builder(pipeline)
								 .Set("Shading.color", Ray::Float4{ 0.f, 1.f, 0.f, 1.f })
								 .Acquire();

		auto blueMaterial = Ray::Material::Builder(std::move(pipeline))
								.Set("Shading.color", Ray::Float4{ 0.f, 0.f, 1.f, 1.f })
								.Acquire();

		m_TransEntity.x.Emplace<Ray::Mesh>(Ray::Mesh::Cylinder(redMaterial, 32, 3.0f));
		m_TransEntity.y.Emplace<Ray::Mesh>(Ray::Mesh::Cylinder(greenMaterial, 32, 3.0f));
		m_TransEntity.z.Emplace<Ray::Mesh>(Ray::Mesh::Cylinder(blueMaterial, 32, 3.0f));
	}
}  // namespace At0::Raditor