#include "Ray/rpch.h"
#include "Ray/Application.h"

#include "Ray/Layers/Layer.h"

// Include Renderer because of incomplete type in Window
#include <RayRender/Renderer3D.h>

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>
#include <RayUtil/Exception.h>


namespace At0::Ray
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string_view commandLine)
		: m_LayerStack{}
	{
		if (s_Instance)
			RAY_ASSERT(false, "Another Application Instance already exists");
		s_Instance = this;

		StartupSetup();
		m_MainWindow = PushWindow(Window::Create("MainWindow", { 100, 100 }, { 960, 540 }));
	}

	void Application::Create(Application* app)
	{
		s_Instance = app;
	}

	void Application::Destroy()
	{
		delete s_Instance;
	}

	Window* Application::FindWindowByName(const std::string_view name)
	{
		for (const Ref<Window>& win : m_WindowStack)
		{
			if (win->GetName() == name)
				return win.get();
		}
		return nullptr;
	}

	int Application::Run()
	{
		auto lastFrameTime = std::chrono::high_resolution_clock::now();
		Timestep timestep;
		
		std::thread appThread([this, &lastFrameTime, &timestep]() 
			{
				while (m_MainWindow->IsOpen())
				{
					// -------------------------------------------------------------------------------------
					// Setting timestep
					auto tNow = std::chrono::high_resolution_clock::now();
					timestep = ((float)(tNow - lastFrameTime).count()) / 1000.0f / 1000.0f / 1000.0f;
					lastFrameTime = std::move(tNow);

					// -------------------------------------------------------------------------------------
					// Update Layers
					for (auto* layer : m_LayerStack)
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
		while (m_MainWindow->IsOpen())
		{
			for (uint32_t i = 0; i < m_WindowStack.Size(); ++i)
			{
				if (m_WindowStack[i]->IsOpen())
				{
					m_WindowStack[i]->Update();
				}
				else
				{
					m_WindowStack.PopIndexed(i);
					--i;
				}
			}
		}
		Cleanup();

		if (appThread.joinable())
			appThread.join();

		return 0;
	}

	// QUESTION: Taking Layer by raw pointer but window by shared_ptr
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


