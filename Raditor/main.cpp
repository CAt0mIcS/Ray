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
#include <Ray/Components/RTagComponent.h>

#include <Ray/Graphics/Images/RTexture2D.h>
#include <Ray/Graphics/Images/RTextureCubemap.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Utils/RException.h>
#include <Ray/Graphics/Pipelines/Shader/RShader.h>
#include <Ray/Core/RDynamicVertex.h>
#include <Ray/Graphics/Buffers/RUniformBuffer.h>

#include <Ray/Scene/RScene.h>
#include <Ray/Scene/RCamera.h>
#include <Ray/Shading/RMaterial.h>

#include <signal.h>
#include <random>
#include <filesystem>

#include <Ray/UI/RImGui.h>
#include <../../Extern/imgui/imgui.h>

#include <Ray/Mono/RScript.h>


namespace At0::Raditor
{
	class Scene : public Ray::Scene
	{
	public:
		Scene() : Ray::Scene(Ray::MakeScope<Ray::Camera>())
		{
			Ray::UInt2 size = Ray::Window::Get().GetFramebufferSize();
			GetCamera().Type = Ray::Camera::LookAt;
			GetCamera().SetPosition(Ray::Float3(0.0f, 0.0f, -2.5f));
			GetCamera().SetRotation(Ray::Float3(0.0f));
			GetCamera().SetRotationSpeed(0.12f);
			GetCamera().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
			GetCamera().SetMovementSpeed(3.0f);
		}
	};

	class App : public Ray::Engine, Ray::EventListener<Ray::CameraChangedEvent>
	{
	public:
		App()
		{
			Ray::Scene::Create<Scene>();
			RegisterForDispatcher(Scene::Get().GetCamera());

			Scene::Get().CreateEntity().Emplace<Ray::Mesh>(
				Ray::Mesh::Import("Resources/Models/Nanosuit/nanosuit.obj"));

			auto pipeline = Ray::GraphicsPipeline::Builder()
								.SetShader(Ray::Shader::Acquire({ "Resources/Shaders/Flat_Col.vert",
									"Resources/Shaders/Flat_Col.frag" }))
								.SetCullMode(VK_CULL_MODE_NONE)
								.Acquire();

			auto material = Ray::Material::Builder(pipeline)
								.Set("Shading.color", Ray::Float4{ .7f, .2f, .2f, 1.f })
								.Acquire();

			m_Pivot = Scene::Get().CreateEntity();
			m_Pivot.Emplace<Ray::Mesh>(Ray::Mesh::Triangle(material));
			m_Pivot.Emplace<Ray::MeshRenderer>(material);
		}

	private:
		virtual void OnEvent(Ray::CameraChangedEvent& e) override
		{
			m_Pivot.Get<Ray::Transform>().SetTranslation(Scene::Get().GetCamera().Pivot);
		}

	private:
		Ray::Entity m_Pivot;
	};

}  // namespace At0::Raditor


void SignalHandler(int signal)
{
	At0::Ray::Log::Critical("Signal {0} received", signal);
	At0::Ray::Log::Close();
}

int main()
{
	using namespace At0;

	signal(SIGABRT, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGINT, SignalHandler);

	Ray::Log::Open("Ray.log");
	Ray::Log::SetLogLevel(Violent::LogLevel::Trace);

	try
	{
		Ray::Log::Info("Launch Path: \"{0}\"", std::filesystem::absolute("."));
		Ray::Window::Create();
		Ray::Window::Get().Show();
		Ray::Window::Get().SetTitle("Raditor");

		return Raditor::App{}.Run();
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