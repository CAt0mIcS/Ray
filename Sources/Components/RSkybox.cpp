#include "Rpch.h"
#include "RSkybox.h"

#include "Graphics/Images/RTexture2D.h"
#include "Graphics/Images/RImage.h"
#include "Graphics/Buffers/RBuffer.h"
#include "RMesh.h"
#include "Scene/RScene.h"

#include <ktx.h>
#include <ktxvulkan.h>


namespace At0::Ray
{
	Skybox::Skybox(Entity entity, Ref<Texture2D> texture) : Component(entity), m_Texture(texture)
	{
		Ray::Mesh& mesh = entity.Emplace<Ray::Mesh>(Ray::Mesh::Import(
			"Resources/Models/UVSphere/UVSphere.obj", Ray::Model::NoNormals,
			Ray::Material{ Ray::Material::LightingTechnique(Ray::Material::LightingTechnique::Flat),
				Ray::Material::Texture2D(texture),
				Ray::Material::CullMode(VK_CULL_MODE_FRONT_BIT) }));
		mesh.GetTransform().SetScale({ Ray::Scene::Get().GetCamera().GetFarClip() - 5.0f,
			Ray::Scene::Get().GetCamera().GetFarClip() - 5.0f,
			Ray::Scene::Get().GetCamera().GetFarClip() - 5.0f });
	}

	void Skybox::Update(Delta dt)
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		Camera& cam = Scene::Get().GetCamera();

		tform.SetScale(
			{ cam.GetFarClip() - 5.0f, cam.GetFarClip() - 5.0f, cam.GetFarClip() - 5.0f });

		Float3 camPos = cam.Position;
		tform.SetTranslation({ -camPos.x, camPos.y, -camPos.z });
	}
}  // namespace At0::Ray
