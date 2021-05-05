#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <UI/RButton.h>

#include <Geometry/RMesh.h>
#include <Graphics/Images/RTexture2D.h>
#include <Graphics/Renderers/RRenderer.h>
#include <Graphics/Renderers/RMeshRenderer.h>
#include <Graphics/Pipelines/RGraphicsPipeline.h>
#include <Utils/RException.h>

#include <Graphics/RVertex.h>
#include <Scene/RScene.h>
#include <Scene/RCamera.h>

#include <signal.h>
#include <random>

#include <Events/REventListener.h>
#include <Events/REngineEvents.h>
#include <Events/RKeyboardEvents.h>
#include <Events/RMouseEvents.h>


using namespace At0;


class Scene : public Ray::Scene
{
public:
	Scene() : Ray::Scene(Ray::MakeScope<Ray::Camera>())
	{
		Ray::UInt2 size = Ray::Window::Get().GetFramebufferSize();
		GetCamera().SetPosition(glm::vec3(0.0f, 0.0f, -2.5f));
		GetCamera().SetRotation(glm::vec3(0.0f));
		GetCamera().SetRotationSpeed(0.07f);
		GetCamera().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
		GetCamera().SetMovementSpeed(3.0f);

		Ray::Renderer::Get().Emplace<Ray::MeshRenderer>();
	}
};


class App :
	public Ray::Engine,
	Ray::EventListener<Ray::HoverEnterEvent>,
	Ray::EventListener<Ray::HoverLeaveEvent>,
	Ray::EventListener<Ray::MouseButtonPressedEvent>,
	Ray::EventListener<Ray::MouseButtonReleasedEvent>
{
public:
	App()
	{
		Ray::Scene::Create<Scene>();

		// Create UI
		Ray::Button& texturedButton = Ray::Scene::Get().CreateEntity().Emplace<Ray::Button>(
			"TexturedButton", Ray::Float2{ 100.0f, 10.0f }, 200.0f, 50.0f,
			Ray::MakeRef<Ray::Texture2D>("Resources/Textures/gridbase.png"));

		Ray::Button& greenButton =
			Ray::Scene::Get().CreateEntity().Emplace<Ray::Button>("GreenButton",
				Ray::Float2{ 100.0f, 200.0f }, 200.0f, 50.0f, Ray::Float3{ 0.0f, 1.0f, 0.0f });
	}

private:
	void OnEvent(Ray::HoverEnterEvent& e) override
	{
		Ray::Log::Warn("HoverEnterEvent {0}", e.GetWidget() ? e.GetWidget()->GetName() : "{Null}");
	}

	void OnEvent(Ray::HoverLeaveEvent& e) override
	{
		Ray::Log::Warn("HoverLeaveEvent {0}", e.GetWidget() ? e.GetWidget()->GetName() : "{Null}");
	}

	void OnEvent(Ray::MouseButtonPressedEvent& e) override
	{
		Ray::Log::Warn(
			"MouseButtonPressedEvent {0}", e.GetWidget() ? e.GetWidget()->GetName() : "{Null}");
	}

	void OnEvent(Ray::MouseButtonReleasedEvent& e) override
	{
		Ray::Log::Warn(
			"MouseButtonReleasedEvent {0}", e.GetWidget() ? e.GetWidget()->GetName() : "{Null}");
	}

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
	Ray::Log::SetLogLevel(Violent::LogLevel::Trace);
#endif

	try
	{
		Ray::Window::Create();
		Ray::Window::Get().Show();
		Ray::Window::Get().SetTitle("UITest");

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
