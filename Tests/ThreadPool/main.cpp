#include <signal.h>
#include <filesystem>

#include <Ray/Utils/RLogger.h>
#include <Ray/Utils/RThreadPool.h>
#include <Ray/Utils/RException.h>
#include <Ray/Components/RTransform.h>
#include <Ray/Components/RTagComponent.h>
#include <Ray/Core/RMath.h>
#include <Ray/Core/RTime.h>
#include <Ray/Scene/RScene.h>
#include <Ray/Core/REngine.h>

#include <../../Extern/entt/src/entt/entt.hpp>

using namespace At0::Ray;

class MyScene : public Scene
{
public:
	MyScene() : Scene(MakeScope<Camera>())
	{
		UInt2 size = Window::Get().GetFramebufferSize();
		GetCamera().SetPosition(Float3(0.0f, 0.0f, -2.5f));
		GetCamera().SetRotation(Float3(0.0f));
		GetCamera().SetRotationSpeed(0.07f);
		GetCamera().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
		GetCamera().SetMovementSpeed(3.0f);
	}
};

uint32_t numEntities = 10000;
void Setup()
{
	for (uint32_t i = 0; i < numEntities; ++i)
	{
		Entity e = Scene::Get().CreateEntity();
		e.Emplace<TagComponent>(std::to_string(i));
	}
}

#define MULTITHREADED 1
void SpeedTest()
{
	// Variables
	static ThreadPool threadPool;

	// Update loop
	if (Window::Get().IsOpen())
	{
		static auto tformView = Scene::Get().EntityView<Transform>();

#if MULTITHREADED
		Log::Debug("Multi-threaded matrix recalculation ({0} entities)", tformView.size());

		Time tStart = Time::Now();
		threadPool.SubmitLoop(0u, (uint32_t)tformView.size(),
			[](uint32_t i) { Entity{ tformView[i] }.Get<Transform>().UpdateMatrix(); });

		threadPool.WaitForTasks();

		Log::Debug("\tTook {0}us", (Time::Now() - tStart).AsMicroseconds());


		// Recalculate matrix on every iteration (not part of profiling)
		for (Entity e : tformView)
			e.Get<Transform>().RecalculateCachedMatrix();
#else
		Log::Debug("Single-threaded matrix recalculation ({0} entities)", tformView.size());

		Time tStart = Time::Now();
		tformView.each([](Transform& tform) { tform.UpdateMatrix(); });
		Log::Debug("\tTook {0}us\n", (Time::Now() - tStart).AsMicroseconds());


		// Recalculate matrix on every iteration (not part of profiling)
		for (Entity e : tformView)
			e.Get<Transform>().RecalculateCachedMatrix();
#endif
	}
}

class App : public Engine
{
public:
	App() { Setup(); }

private:
	void Update() override { SpeedTest(); }
};


void SignalHandler(int signal)
{
	Log::Critical("Signal {0} received", signal);
	Log::Close();
}

int main()
{
	signal(SIGABRT, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGINT, SignalHandler);

	Log::Open("Ray.log");
	Log::SetLogLevel(At0::Violent::LogLevel::Trace);

	try
	{
		Log::Info("Launch Path: \"{0}\"", std::filesystem::absolute("."));

		Window::Create();
		Scene::Create<MyScene>();

		return App{}.Run();
	}
	catch (Exception& e)
	{
		Log::Critical("{0}: {1}", e.GetType(), e.what());
	}
	catch (std::exception& e)
	{
		Log::Critical("Standard Exception: {0}", e.what());
	}
	catch (...)
	{
		Log::Critical("Unknown exception occured.");
	}
}
