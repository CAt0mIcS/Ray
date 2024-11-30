#include "REngine.h"

#include "Devices/RWindow.h"
#include "Graphics/RGraphics.h"

#include "Scene/RScene.h"


namespace At0::Ray
{
	Engine::~Engine()
	{
		Log::Info("[Engine] Shutdown");
	}

	int Engine::Run()
	{
		Log::Info("[Engine] Startup");

		auto startSecTime = std::chrono::high_resolution_clock::now();
		while (UpdateWindows())
		{
			Update();
		}
		return 0;
	}

	Time Engine::GetDelta(const std::string& tag) const
	{
		RAY_MEXPECTS(
			m_Windows.find(tag) != m_Windows.end(), "Window with tag {0} does not exist", tag);
		return m_Windows.at(tag).deltaTime.Change();
	}

	uint32_t Engine::GetFPS(const std::string& tag) const
	{
		RAY_MEXPECTS(
			m_Windows.find(tag) != m_Windows.end(), "Window with tag {0} does not exist", tag);
		return m_Windows.at(tag).fps.Value();
	}

	Ref<Window> Engine::CreateWindow(const std::string& tag)
	{
		RAY_MEXPECTS(!tag.empty(), "Invalid Window tag");
		RAY_MEXPECTS(GetNumWindows() <= s_MaxWindows, "Maximum number of windows has been created");

		return m_Windows.emplace(tag, MakeRef<Window>(m_Context.instance, m_Context.physicalDevice))
			.first->second.window;
	}

	void Engine::DestroyWindow(const std::string& tag)
	{
		RAY_MEXPECTS(
			m_Windows.find(tag) != m_Windows.end(), "Window with tag {0} does not exist", tag);
		m_Windows.erase(tag);
	}

	Ref<Scene> Engine::CreateScene(const std::string& tag)
	{
		return m_Resources.Emplace<Scene>(tag, *this,
			MakeScope<Camera>(*m_Windows["MainWindow"].window));  // RAY_TEMPORARY: Window
	}

	void Engine::DestroyScene(const std::string& tag)
	{
		m_Resources.Destroy(tag);
	}

	void Engine::SetActiveScene(Ref<Window> window, Ref<Scene> scene)
	{
		window->SetActiveScene(scene);
		scene->SetWindow(window);
	}

	Engine::Engine()
	{
		// RAY_TODO: References to windows would get invalidated if rehash occurrs. Reserving max
		// size to avoid that
		m_Windows.reserve(s_MaxWindows);
	}

	bool Engine::UpdateWindows()
	{
		bool keepEngineRunning = false;

		for (auto& [tag, data] : m_Windows)
		{
			if (data.window->Update(data.deltaTime) && !keepEngineRunning)
				keepEngineRunning = true;

			data.deltaTime.Update();
			data.fps.Update(Time::Now());
		}

		return keepEngineRunning;
	}
}  // namespace At0::Ray
