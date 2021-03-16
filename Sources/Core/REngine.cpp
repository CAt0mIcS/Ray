#include "Rpch.h"
#include "REngine.h"

#include "Devices/RWindow.h"
#include "Utils/RLogger.h"
#include "Utils/RAssert.h"

#include "Graphics/RGraphics.h"


namespace At0::Ray
{
	Engine* Engine::s_Instance = nullptr;

	Engine& Engine::Get()
	{
		RAY_MEXPECTS(s_Instance, "[Engine::Get] Engine not initialized");
		return *s_Instance;
	}

	int Engine::Run()
	{
		Log::Info("[Engine] Startup");

		auto startSecTime = std::chrono::high_resolution_clock::now();
		while (Window::Get().Update())
		{
			m_Delta.Update();
			m_FPS.Update(Time::Now());
			Graphics::Get().Update(m_Delta);
			Update();


			if (std::chrono::duration<float, std::chrono::milliseconds::period>(
					std::chrono::high_resolution_clock::now() - startSecTime)
					.count() > 500)
			{
				std::ostringstream oss;
				oss << "Frametime: " << m_Delta.Change().AsSeconds() << "s"
					<< ", FPS: " << m_FPS.Value();
				Window::Get().SetTitle(oss.str());
				Log::Debug("{0}", oss.str());

				startSecTime = std::chrono::high_resolution_clock::now();
			}
		}

		Graphics::Destroy();

		Log::Info("[Engine] Shutdown");
		return 0;
	}

	Engine::Engine()
	{
		RAY_MEXPECTS(!s_Instance, "[Engine] Already initialized");
		s_Instance = this;
	}
}  // namespace At0::Ray
