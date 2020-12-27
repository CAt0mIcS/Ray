#include "Ray/rpch.h"
#include "Ray/RApplication.h"

#include "Ray/Layers/RLayer.h"
#include "Ray/Core/RThreadPool.h"

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

template<size_t arrSize>
void SpeedTest(int num)
{
	using namespace At0::Ray;

	float avgCreationTimeECS = 0.0f;
	float avgCreationTimeENTT = 0.0f;
	float avgGetTimeECS = 0.0f;
	float avgGetTimeENTT = 0.0f;
	float avgViewTimeECS = 0.0f;
	float avgViewTimeENTT = 0.0f;
	for (size_t i = 0; i <= num; ++i)
	{
		std::cout << "=========================== ECS =========================== \n";

		{
			auto start = std::chrono::high_resolution_clock::now();

			ECS::Registry registry;
			ECS::Entity* entities = new ECS::Entity[arrSize];

			for (uint32_t i = 0; i < arrSize; ++i)
			{
				entities[i] = registry.Create();
				registry.Emplace<TransformComponent>(entities[i], 3245.423f);
				registry.Emplace<TagComponent>(entities[i], "Tag " + std::to_string(i));
				registry.Emplace<SpriteRenderComponent>(entities[i], 332);
				registry.Emplace<RenderComponent>(entities[i], 32);
			}
			auto end = std::chrono::high_resolution_clock::now();
			avgCreationTimeECS += (end - start).count() / 1000.0f / 1000.0f;
			std::cout << "Creation: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";

			start = std::chrono::high_resolution_clock::now();
			for (uint32_t i = 0; i < arrSize; ++i)
			{
				TransformComponent& tForm = registry.Get<TransformComponent>(entities[i]);
			}
			end = std::chrono::high_resolution_clock::now();
			avgGetTimeECS += (end - start).count() / 1000.0f / 1000.0f;
			std::cout << "Get: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";

			start = std::chrono::high_resolution_clock::now();
			auto view = registry.View<TransformComponent, TagComponent, SpriteRenderComponent>();
			for (auto entity : view)
			{
				TransformComponent& tform = registry.Get<TransformComponent>(entity);
				TagComponent& tag = registry.Get<TagComponent>(entity);
				SpriteRenderComponent& spriteRender = registry.Get<SpriteRenderComponent>(entity);
			}
			end = std::chrono::high_resolution_clock::now();
			avgViewTimeECS += (end - start).count() / 1000.0f / 1000.0f;
			std::cout << "View: " << (end - start).count() / 1000.0f / 1000.0f << "ms\n";
		}
		std::cout << "\n\n=========================== NEW TEST =========================== \n";
	}

	std::cout << "\n\n=========================== RESULT =========================== \n";

	std::cout << "ECS: \n\tAverage Creation Time: " << avgCreationTimeECS / num;
	std::cout << "\n\tAverage Get Time: " << avgGetTimeECS / num;
	std::cout << "\n\tAverage View Time: " << avgViewTimeECS / num;
}


namespace At0::Ray
{
	Application* Application::s_Instance = nullptr;

	Application::Application(std::string_view commandLine)
		: m_LayerStack{}
	{
		RAY_MEXPECTS(!s_Instance, "Another Application Instance already exists at 0x{0}", s_Instance);
		s_Instance = this;

		StartupSetup();
		PushWindow(Window::Create("MainWindow", { 100, 100 }, { 960, 540 }));

		// -------------------------------------------------------------
		// ECS TESTING
		//SpeedTest<5000000>(10);
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
		std::thread appThread([this]()
			{
				auto lastFrameTime = std::chrono::high_resolution_clock::now();
				while (GetMainWindow().IsOpen())
				{
					// -------------------------------------------------------------------------------------
					// Setting timestep
					auto tNow = std::chrono::high_resolution_clock::now();
					Timestep timestep = ((float)(tNow - lastFrameTime).count()) / 1000.0f / 1000.0f / 1000.0f;
					lastFrameTime = tNow;

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

	Layer& Application::PushLayer(Layer* layer)
	{
		RAY_PROFILE_FUNCTION();

		m_LayerStack.PushBack(layer);
		return *layer;
	}

	Window& Application::PushWindow(Ref<Window> window)
	{
		RAY_PROFILE_FUNCTION();

		m_WindowStack.PushBack(window);
		return *window.get();
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


