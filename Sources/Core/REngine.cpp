#include "Rpch.h"
#include "REngine.h"

#include "Devices/RWindow.h"
#include "Utils/RLogger.h"
#include "Utils/RAssert.h"


namespace At0::Ray
{
	Engine* Engine::s_Instance = nullptr;

	Engine& Engine::Get()
	{
		RAY_MEXPECTS(s_Instance, "[Engine::Get] Engine not initialized.");
		return *s_Instance;
	}

	int Engine::Run()
	{
		while (Window::Get().Update())
		{
			m_Delta.Update();
			m_FPS.Update(Time::Now());
			Update();
		}

		return 0;
	}

	Engine::Engine()
	{
		RAY_MEXPECTS(!s_Instance, "[Engine] Already initialized.");
		s_Instance = this;
	}
}  // namespace At0::Ray
