#include "Ray/Rpch.h"
#include "Ray/RApplication.h"

#include "Ray/Layers/RLayer.h"
#include "Ray/Core/RThreadPool.h"

// Include Renderer because of incomplete type in Window
#include <RayRender/RRenderer3D.h>

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>
#include <RayUtil/RException.h>
#include <RayUtil/RHelper.h>


namespace At0::Ray
{
	Application* Application::s_Instance = nullptr;
	std::string Application::s_CommandLine = "";

	Application::Application(std::string_view commandLine)
		: m_LayerStack{}
	{
		RAY_MEXPECTS(!s_Instance, "Another Application Instance already exists at 0x{0}", s_Instance);
		s_Instance = this;

		StartupSetup();
		PushWindow(Window::Create("MainWindow", { 100, 100 }, { 960, 540 }));
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
		//std::thread appThread([this]()
		//	{
		//		auto lastFrameTime = std::chrono::high_resolution_clock::now();
		//		while (GetMainWindow().IsOpen())
		//		{
		//			// -------------------------------------------------------------------------------------
		//			// Setting timestep
		//			auto tNow = std::chrono::high_resolution_clock::now();
		//			Timestep timestep = ((float)(tNow - lastFrameTime).count()) / 1000.0f / 1000.0f / 1000.0f;
		//			lastFrameTime = tNow;

		//			// -------------------------------------------------------------------------------------
		//			// Update Layers
		//			for (Layer* layer : m_LayerStack)
		//			{
		//				layer->OnUpdate(timestep);
		//			}

		//			//CPU Usage too high without it (not ideal)
		//			//std::this_thread::sleep_for(std::chrono::nanoseconds(1500));
		//		}
		//	}
		//);

		// -----------------------------------------------------------------------------------------
		// Main Application Loop
		auto lastFrameTime = std::chrono::high_resolution_clock::now();
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
		}
		Cleanup();

		//if (appThread.joinable())
		//	appThread.join();

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
		Log::Debug("[Application] Destroyed");
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
				Log::Critical("Signal '{0}' received, terminating program", signum);
				Log::End();
				Profile::EndSession();

				exit(signum);
			}
		);
	}
}


