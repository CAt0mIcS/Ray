#include "Ray/rpch.h"
#include "Ray/RApplication.h"

#include "Ray/Layers/RLayer.h"

// Include Renderer because of incomplete type in Window
#include <RayRender/Renderer3D.h>

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>
#include <RayUtil/RException.h>

// -------------------------------------------------------------
// ECS TESTING INCLUDES
#include "RayECS\RayECS.h"
#include "Ray/Core/RComponents.h"


namespace At0::Ray
{
	Application* Application::s_Instance = nullptr;

	Application::Application(std::string_view commandLine)
		: m_LayerStack{}
	{
		if (s_Instance)
			RAY_ASSERT(false, "Another Application Instance already exists");
		s_Instance = this;

		StartupSetup();
		PushWindow(Window::Create("MainWindow", { 100, 100 }, { 960, 540 }));

		// -------------------------------------------------------------
		// ECS TESTING
		ECS::Registry registry;
		ECS::Entity e = registry.Create();
		ECS::Entity e2 = registry.Create();
		registry.Emplace<TagComponent>(e, "This is the first entities tag component");
		registry.Emplace<TagComponent>(e2, "This is the second entities tag component");
		registry.Emplace<TransformComponent>(e, 3.33f);
		registry.Emplace<TransformComponent>(e2, 4.44f);

		ECS::ComponentView<TagComponent, TransformComponent> view = registry.View<TagComponent, TransformComponent>();

		registry.View<TagComponent, TransformComponent>().Each([&](ECS::Entity e) { std::cout << registry.Get<TagComponent>(e).tag << '\n'; });

		if (registry.Any<TagComponent, TransformComponent>(e))
		{
			std::cout << "Entity has\n";
		}

		for (ECS::Entity e : view)
		{
			std::cout << registry.Get<TransformComponent>(e).x << '\n';
			std::cout << registry.Get<TagComponent>(e).tag << '\n';
		}


	}

	Window& Application::FindWindowByName(std::string_view name)
	{
		for (const Ref<Window>& win : m_WindowStack)
		{
			if (win->GetName() == name)
				return *win.get();
		}

		RAY_THROW_RUNTIME("[Application::FindWindowByName] was unable to find the Window with name '{0}'.", name);
	}

	int Application::Run()
	{
		auto lastFrameTime = std::chrono::high_resolution_clock::now();
		Timestep timestep;
		
		std::thread appThread([this, &lastFrameTime, &timestep]() 
			{
				while (GetMainWindow().IsOpen())
				{
					// -------------------------------------------------------------------------------------
					// Setting timestep
					auto tNow = std::chrono::high_resolution_clock::now();
					timestep = ((float)(tNow - lastFrameTime).count()) / 1000.0f / 1000.0f / 1000.0f;
					lastFrameTime = std::move(tNow);

					// -------------------------------------------------------------------------------------
					// Update Layers
					for (Layer* layer : m_LayerStack)
					{
						layer->OnUpdate(timestep);
					}

					//CPU Usage too high without it (not ideal)
					//std::this_thread::sleep_for(std::chrono::nanoseconds(1500));
				}
			}
		);

		// -----------------------------------------------------------------------------------------
		// Main Application Loop
		while (GetMainWindow().IsOpen())
		{
			for (int16_t i = m_WindowStack.Size() - 1; i >= 0; --i)
			{
				if (m_WindowStack[i]->IsOpen())
				{
					m_WindowStack[i]->Update();
				}
				else
				{
					m_WindowStack.PopIndexed(i);
				}
			}
		}
		Cleanup();

		if (appThread.joinable())
			appThread.join();

		return 0;
	}

	// QUESTIONA: Taking Layer by raw pointer but window by shared_ptr (use shared_ptr)
	//RAY_TODO: Generalize for the entire project
	Layer* Application::PushLayer(Layer* layer)
	{
		RAY_PROFILE_FUNCTION();

		m_LayerStack.PushBack(layer);
		return layer;
	}

	Window* Application::PushWindow(Ref<Window> window)
	{
		RAY_PROFILE_FUNCTION();

		m_WindowStack.PushBack(window);
		return window.get();
	}

	Application::~Application()
	{
		RAY_PROFILE_FUNCTION();
		RAY_LOG_DEBUG("[Application] Destroyed");
	}
	
	void Application::Cleanup()
	{
		// Close all Windows when the MainWindow was closed
		for (auto& window : m_WindowStack)
		{
			window->Close();
		}

		// Need this here to stop threads from waiting until the static ThreadPool object is destroyed
		ThreadPool::Get().Shutdown();
	}
	
	void Application::StartupSetup()
	{
		/// <summary>
		/// Handles any unexpected signals, logs the signal and closes the logger
		/// </summary>
		/// <param name="signum">Is the received signal</param>
		Util::SetSignals([](int signum)
			{
				RAY_LOG_CRITICAL("Signal '{0}' received, terminating program", signum);
				RAY_LOG_END();
				RAY_PROFILE_END_SESSION();

				exit(signum);
			}
		);
		Util::ConsoleSetup();
	}
}


