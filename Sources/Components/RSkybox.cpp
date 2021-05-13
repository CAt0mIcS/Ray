#include "Rpch.h"
#include "RSkybox.h"

#include "Graphics/Images/RTexture2D.h"
#include "Graphics/Images/RTextureCubemap.h"
#include "Graphics/Images/RImage.h"
#include "Graphics/Buffers/RBuffer.h"
#include "RMesh.h"
#include "Scene/RScene.h"


namespace At0::Ray
{
	// RAY_TODO: Disable depth stencil for skybox

	Skybox::Skybox(Entity entity, Ref<Texture2D> texture) : Component(entity)
	{
		Material::Layout layout{};
		layout.shaders = { "Resources/Shaders/Flat_Tex.vert", "Resources/Shaders/Flat_Tex.frag" };
		layout.cullMode = VK_CULL_MODE_FRONT_BIT;
		layout.diffuseMap = texture;

		Mesh& mesh = GetEntity().Emplace<Mesh>(
			Mesh::Import("Resources/Models/UVSphere/UVSphere.obj", std::move(layout)));
		mesh.GetTransform().SetScale(Float3{ Scene::Get().GetCamera().GetFarClip() - 5.0f });

		mesh.GetMaterial().AddBufferUniform("PerObjectData", Shader::Stage::Vertex);
		mesh.GetMaterial().AddSampler2DUniform("texSampler", Shader::Stage::Fragment, texture);
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
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		Camera& cam = Scene::Get().GetCamera();

		tform.SetScale(
			{ cam.GetFarClip() - 5.0f, cam.GetFarClip() - 5.0f, cam.GetFarClip() - 5.0f });

		Float3 camPos = cam.Position;
		tform.SetTranslation({ -camPos.x, camPos.y, -camPos.z });
	}
}  // namespace At0::Ray
