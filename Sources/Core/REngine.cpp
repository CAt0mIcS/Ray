#include "Rpch.h"
#include "REngine.h"

#include "Devices/RWindow.h"
#include "Debug/RLogger.h"


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
		}

		return 0;
	}

	Time Engine::GetDelta() const
	{
		return m_Delta.Change();
	}

	Engine::Engine()
	{
		RAY_MEXPECTS(!s_Instance, "[Engine] Already initialized.");
		s_Instance = this;
	}
}  // namespace At0::Ray
