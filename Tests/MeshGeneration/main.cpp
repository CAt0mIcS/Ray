#include <Ray/Core/REngine.h>
#include <Ray/Utils/RLogger.h>
#include <Ray/Devices/RWindow.h>
#include <Ray/Graphics/RGraphics.h>

#include <Ray/UI/RButton.h>

#include <Ray/Components/RMesh.h>
#include <Ray/Components/RMeshRenderer.h>
#include <Ray/Components/RTransform.h>
#include <Ray/Components/RSkybox.h>
#include <Ray/Components/RScriptableEntity.h>

#include <Ray/Graphics/Images/RTexture.h>
#include <Ray/Graphics/Images/RTextureCubemap.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Utils/RException.h>
#include <Ray/Graphics/Pipelines/Shader/RShader.h>
#include <Ray/Shading/RMaterial.h>
#include <Ray/Core/RDynamicVertex.h>

#include <Ray/Scene/RScene.h>
#include <Ray/Scene/RCamera.h>

#include <signal.h>
#include <random>
#include <filesystem>

#include <Ray/UI/RImGui.h>
#include <../../Extern/imgui/imgui.h>


using namespace At0;


class Scene : public Ray::Scene
{
public:
	Scene() : Ray::Scene(Ray::MakeScope<Ray::Camera>())
	{
		Ray::UInt2 size = Ray::Window::Get().GetFramebufferSize();
		GetCamera().SetPosition(Ray::Float3(0.0f, 0.0f, -2.5f));
		GetCamera().SetRotation(Ray::Float3(0.0f));
		GetCamera().SetRotationSpeed(0.07f);
		GetCamera().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
		GetCamera().SetMovementSpeed(3.0f);
	}
};


class App : public Ray::Engine
{
public:
	App()
	{
		Ray::Scene::Create<Scene>();
		RAY_IMGUI(
			[&]()
			{
				{
					auto translate = [](Ray::Transform& tform)
					{
						Ray::Float3 newTranslation =
							Ray::ImGUI::Float3Widget("Translation", tform.Translation());
						Ray::Float3 newRotation =
							Ray::ImGUI::Float3Widget("Rotation", tform.Rotation());
						Ray::Float3 newScale = Ray::ImGUI::Float3Widget("Scale", tform.Scale());

						if (newTranslation != tform.Translation())
							tform.SetTranslation(newTranslation);
						if (newRotation != tform.Rotation())
							tform.SetRotation(newRotation);
						if (newScale != tform.Scale())
							tform.SetScale(newScale);
					};
				}
			});

#include "../ImGuiWindows.inl"

		auto noCullPipeline =
			Ray::GraphicsPipeline::Builder()
				.SetShader(Ray::Shader::AcquireSourceFile(
					{ "Resources/Shaders/Flat_Col.vert", "Resources/Shaders/Flat_Col.frag" }))
				.SetCullMode(VK_CULL_MODE_NONE)
				.Acquire();

		auto cullPipeline =
			Ray::GraphicsPipeline::Builder()
				.SetShader(Ray::Shader::AcquireSourceFile(
					{ "Resources/Shaders/Flat_Col.vert", "Resources/Shaders/Flat_Col.frag" }))
				.Acquire();

		auto noCullMaterial = Ray::Material::Builder(std::move(noCullPipeline))
								  .Set("Shading.color", Ray::Float4{ 0.0f, 1.0f, 1.0f, 1.0f })
								  .Acquire();

		auto cullMaterial = Ray::Material::Builder(std::move(cullPipeline))
								.Set("Shading.color", Ray::Float4{ 1.f, 0.f, 0.f, 1.f })
								.Acquire();

		auto texturePipeline =
			Ray::GraphicsPipeline::Builder()
				.SetShader(Ray::Shader::AcquireSourceFile(
					{ "Resources/Shaders/Flat_Diff.vert", "Resources/Shaders/Flat_Diff.frag" }))
				.SetCullMode(VK_CULL_MODE_NONE)
				.Acquire();

		auto textureMaterial =
			Ray::Material::Builder(std::move(texturePipeline))
				.Set("samplerDiffuse",
					Ray::Texture::Acquire("Resources/Textures/EquirectangularWorldMap.jpg"))
				.Acquire();

		Ray::Entity plane = Scene::Get().CreateEntity();
		plane.Emplace<Ray::Mesh>(Ray::Mesh::Plane(textureMaterial));
		plane.Get<Ray::Transform>().Translate({ -15.f, 0.f, 0.f });

		Ray::Entity triangle = Scene::Get().CreateEntity();
		triangle.Emplace<Ray::Mesh>(Ray::Mesh::Triangle(noCullMaterial));
		triangle.Get<Ray::Transform>().Translate({ -10.f, 0.f, 0.f });

		Ray::Entity halfCircle = Scene::Get().CreateEntity();
		halfCircle.Emplace<Ray::Mesh>(Ray::Mesh::HalfCircle(noCullMaterial, 6, 2.0f));
		halfCircle.Get<Ray::Transform>().Translate({ -5.f, 0.f, 0.f });

		Ray::Entity circle = Scene::Get().CreateEntity();
		circle.Emplace<Ray::Mesh>(Ray::Mesh::Circle(noCullMaterial, 32, 2.0f));
		circle.Get<Ray::Transform>().Translate({ 0.f, 0.f, 0.f });

		Ray::Entity cube = Scene::Get().CreateEntity();
		cube.Emplace<Ray::Mesh>(Ray::Mesh::Cube(cullMaterial));
		cube.Get<Ray::Transform>().Translate({ 5.f, 0.f, 0.f });

		Ray::Entity uvsphere = Scene::Get().CreateEntity();
		uvsphere.Emplace<Ray::Mesh>(Ray::Mesh::UVSphere(cullMaterial, 2.0f, 128, 128));
		uvsphere.Get<Ray::Transform>().Translate({ 10.f, 0.f, 0.f });

		Ray::Entity cylinder = Scene::Get().CreateEntity();
		cylinder.Emplace<Ray::Mesh>(Ray::Mesh::Cylinder(cullMaterial, 64, 1.f));
		cylinder.Get<Ray::Transform>().Translate({ 15.f, 0.f, 0.f });

		Ray::Entity cone = Scene::Get().CreateEntity();
		cone.Emplace<Ray::Mesh>(Ray::Mesh::Cone(cullMaterial, 32, 1.f));
		cone.Get<Ray::Transform>().Translate({ 20.f, 0.f, 0.f });

		Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
			Ray::MakeRef<Ray::Texture>("Resources/Textures/EquirectangularWorldMap.jpg"));
	}

private:
	void Update() override {}
};

void SignalHandler(int signal)
{
	Ray::Log::Critical("Signal {0} received", signal);
	Ray::Log::Close();
}

int main()
{
	signal(SIGABRT, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGINT, SignalHandler);

	Ray::Log::Open("Ray.log");
#ifdef NDEBUG
	Ray::Log::SetLogLevel(Violent::LogLevel::Information);
#else
	Ray::Log::SetLogLevel(Violent::LogLevel::Information);
#endif

	try
	{
		Ray::Log::Info("Launch Path: \"{0}\"", std::filesystem::absolute("."));
		Ray::Window::Create();
		Ray::Window::Get().Show();
		Ray::Window::Get().SetTitle("SetupTest");

		return App{}.Run();
	}
	catch (Ray::Exception& e)
	{
		Ray::Log::Critical("{0}: {1}", e.GetType(), e.what());
	}
	catch (std::exception& e)
	{
		Ray::Log::Critical("Standard Exception: {0}", e.what());
	}
	catch (...)
	{
		Ray::Log::Critical("Unknown exception occured.");
	}
}
