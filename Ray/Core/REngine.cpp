#include "REngine.h"

#include "Devices/RWindow.h"
#include "Graphics/RGraphics.h"

#include "Scene/RScene.h"


namespace At0::Ray
{
	Engine* Engine::s_Instance = nullptr;

	Engine::~Engine()
	{
		Graphics::Destroy();
		Log::Info("[Engine] Shutdown");
	}

	Engine& Engine::Get()
	{
		RAY_MEXPECTS(s_Instance, "[Engine::Get] Engine not initialized");
		return *s_Instance;
	}

	int Engine::Run()
	{
		Log::Info("[Engine] Startup");

		auto startSecTime = std::chrono::high_resolution_clock::now();
		while (m_MainWindow.Update(m_Delta))
		{
			m_Delta.Update();
			m_FPS.Update(Time::Now());
			Update();
		}
		return 0;
	}

	Engine::Engine()
		: m_VulkanInstance{}, m_PhysicalDevice{ m_VulkanInstance },
		  m_MainWindow{ GetRenderContext() }
	{
		RAY_MEXPECTS(!s_Instance, "[Engine] Already initialized");
		s_Instance = this;
	}
}  // namespace At0::Ray
