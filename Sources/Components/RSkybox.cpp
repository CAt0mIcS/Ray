#include "Rpch.h"
#include "RSkybox.h"

#include "Graphics/Images/RTexture2D.h"
#include "Graphics/Images/RTextureCubemap.h"
#include "Graphics/Images/RImage.h"
#include "Graphics/Buffers/RBuffer.h"

#include "Scene/RScene.h"
#include "Shading/Flat/RFlatTextureMaterial.h"

#include "RMesh.h"
#include "RTransform.h"
#include "RMeshRenderer.h"


namespace At0::Ray
{
	// RAY_TODO: Disable depth stencil for skybox

	Skybox::Skybox(Entity entity, Ref<Texture2D> texture) : Component(entity)
	{
		GraphicsPipeline::Layout layout{};
		layout.cullMode = VK_CULL_MODE_FRONT_BIT;
		Ref<Material> material = MakeRef<FlatTextureMaterial>(
			FlatTextureMaterial::Layout{ std::move(texture) }, std::move(layout));

		Mesh& mesh = GetEntity().Emplace<Mesh>(
			Mesh::Import("Resources/Models/UVSphere/UVSphere.obj", material));
		GetEntity().Get<Transform>().SetScale(
			Float3{ Scene::Get().GetCamera().GetFarClip() - 5.0f });
	}

	Skybox::Skybox(Entity entity, Ref<TextureCubemap> texture) : Component(entity)
	{
		// Mesh& mesh = GetEntity().Emplace<Mesh>(Mesh::Import("Resources/Models/Cube/Cube.obj",
		//	Model::NoNormals | Model::NoTextureCoordinates,
		//	Material{ Material::LightingTechnique(Material::LightingTechnique::Flat),
		//		Material::Color({ 0.0f, 1.0f, 0.0f }),
		//		Material::CullMode(VK_CULL_MODE_FRONT_BIT) }));
		// mesh.GetTransform().SetScale(Float3{ Scene::Get().GetCamera().GetFarClip() - 5.0f });
	}

	void Skybox::Update(Delta dt)
	{
		Transform& tform = GetEntity().Get<Transform>();
		Camera& cam = Scene::Get().GetCamera();

		tform.SetScale(
			{ cam.GetFarClip() - 5.0f, cam.GetFarClip() - 5.0f, cam.GetFarClip() - 5.0f });

		Float3 camPos = cam.Position;
		tform.SetTranslation({ -camPos.x, camPos.y, -camPos.z });
	}
}  // namespace At0::Ray
