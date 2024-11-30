#pragma once

#include "../RBase.h"
#include "RTime.h"

#include "../Graphics/Core/RVulkanInstance.h"
#include "../Graphics/Core/RPhysicalDevice.h"

#include "../Core/RResourceManager.h"


namespace At0::Ray
{
	class Scene;
	class Window;

	class RAY_EXPORT Engine
	{
	public:
		virtual ~Engine();

		/**
		 * Starts the application run loop
		 * @returns the exit code
		 */
		int Run();

		/**
		 * @returns the time since the last frame of the window with tag
		 */
		Time GetDelta(const std::string& tag) const;

		/**
		 * @returns The FPS of the window with tag
		 */
		uint32_t GetFPS(const std::string& tag) const;

		/**
		 * Creates a new window with tag and stores it
		 */
		Ref<Window> CreateWindow(const std::string& tag);

		/**
		 * Destroys existing window and frees up all resources belonging to it
		 */
		void DestroyWindow(const std::string& tag);

		/**
		 * Creates a new empty scene using a derived scene
		 */
		template<typename T, typename... Args>
		Ref<T> CreateScene(const std::string& tag, Args&&... args)
		{
			m_Resources.Emplace(tag, std::forward<Args>(args)...);
		}

		/**
		 * Creates a new empty scene
		 */
		Ref<Scene> CreateScene(const std::string& tag);
		void DestroyScene(const std::string& tag);

		size_t GetNumWindows() const { return m_Windows.size(); }
		Ref<Window> GetWindow(const std::string& tag) { return m_Windows.at(tag).window; }
		const Window& GetWindow(const std::string& tag) const { return *m_Windows.at(tag).window; }

		ResourceManager& GetResourceManager() { return m_Resources; }
		const ResourceManager& GetResourceManager() const { return m_Resources; }

		/**
		 * First thing updated for now
		 */
		virtual void Update() {}

	public:
		struct RenderContext
		{
			VulkanInstance instance{};
			PhysicalDevice physicalDevice{ instance };
		};

	protected:
		Engine();

	private:
		bool UpdateWindows();

	private:
		struct WindowData
		{
			Ref<Window> window;
			Delta deltaTime;
			ChangePerSecond fps;
		};

	private:
		static constexpr int s_MaxWindows = 12;

		Engine::RenderContext m_Context{};
		std::unordered_map<std::string, WindowData> m_Windows;
		ResourceManager m_Resources;
	};
}  // namespace At0::Ray
