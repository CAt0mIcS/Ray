#pragma once

#include "../RBase.h"
#include "RTime.h"

#include "../Graphics/Core/RVulkanInstance.h"
#include "../Graphics/Core/RPhysicalDevice.h"
#include "../Graphics/Core/RRenderContext.h"

#include "../Devices/RWindow.h"


namespace At0::Ray
{
	class RAY_EXPORT Engine
	{
	public:
		virtual ~Engine();

		/**
		 * @returns The static application instance
		 */
		static Engine& Get();

		/**
		 * Starts the application run loop
		 * @returns the exit code
		 */
		int Run();

		/**
		 * @returns the time since the last frame
		 */
		Time GetDelta() const { return m_Delta.Change(); }

		/**
		 * @returns The FPS
		 */
		uint32_t GetFPS() const { return m_FPS.Value(); }

		EngineRenderContext GetRenderContext()
		{
			return EngineRenderContext{ m_VulkanInstance, m_PhysicalDevice };
		}

		const Window& GetMainWindow() const { return m_MainWindow; }
		Window& GetMainWindow() { return (Window&)std::as_const(*this).GetMainWindow(); }

		/**
		 * First thing updated for now
		 */
		virtual void Update() {}

	protected:
		Engine();

	private:
		static Engine* s_Instance;
		Delta m_Delta{};
		ChangePerSecond m_FPS{};

		VulkanInstance m_VulkanInstance;
		PhysicalDevice m_PhysicalDevice;

		Window m_MainWindow;
	};
}  // namespace At0::Ray
