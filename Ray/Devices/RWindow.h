#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"
#include "../Core/RTime.h"

#include "../Graphics/Core/RRenderContext.h"

#include "../Events/REventDispatcher.h"
#include "../Events/REngineEvents.h"
#include "../Events/RMouseEvents.h"
#include "../Events/RKeyboardEvents.h"

#include "../Utils/RImGui.h"

#include <string_view>
#include <utility>
#include <stdint.h>

#include <vulkan/vulkan_core.h>

struct GLFWwindow;


namespace At0::Ray
{
	class Scene;
	class Widget;
	class RenderContext;
	class VulkanInstance;
	class PhysicalDevice;

	class RAY_EXPORT Window :
		public EventDispatcher<WindowResizedEvent>,
		public EventDispatcher<FramebufferResizedEvent>,
		public EventDispatcher<WindowMovedEvent>,
		public EventDispatcher<WindowClosedEvent>,
		public EventDispatcher<MouseButtonPressedEvent>,
		public EventDispatcher<MouseButtonReleasedEvent>,
		public EventDispatcher<MouseMovedEvent>,
		public EventDispatcher<ScrollUpEvent>,
		public EventDispatcher<ScrollDownEvent>,
		public EventDispatcher<ScrollLeftEvent>,
		public EventDispatcher<ScrollRightEvent>,
		public EventDispatcher<KeyPressedEvent>,
		public EventDispatcher<KeyReleasedEvent>,
		public EventDispatcher<KeyRepeatedEvent>,
		public EventDispatcher<CharEvent>,
		public EventDispatcher<HoverEnterEvent>,
		public EventDispatcher<HoverLeaveEvent>
	{
	public:
		/**
		 * Creates the Window. Call Window::Show to show it
		 */
		Window(const VulkanInstance& vkInstance, const PhysicalDevice& physicalDevice,
			uint32_t width = 960, uint32_t height = 540, std::string_view title = "Window");

		/**
		 * Shows the window
		 */
		void Show() const;

		/**
		 * Hides the window so that it can be shown again
		 */
		void Hide() const;

		/**
		 * Closes the window. Cannot be opened again (use Window::Hide if required)
		 */
		void Close();

		/**
		 * @returns If the cursor is currently enabled
		 */
		bool CursorEnabled() const;

		/**
		 * Enables the cursor if it was disabled and frees the cursor clipping
		 */
		void EnableCursor() const;

		/**
		 * Disables the cursor and clips the cursor to the window rect
		 */
		void DisableCursor() const;

		/**
		 * @returns If the window is minimzed
		 */
		bool Minimized() const;

		/**
		 * @returns If the window is shown
		 */
		bool IsOpen() const;

		/*
		 * Polls events and updates everything (graphics engine)
		 * @returns If the window should stay open
		 */
		bool Update(Delta dt);

		/**
		 * Querys required extensions for Vulkan
		 * @returns All extensions that the Vulkan instance needs
		 */
		static std::pair<const char**, uint32_t> GetInstanceExtensions();

		/**
		 * Creates the surface to render to
		 */
		void CreateSurface(VkInstance instance, const VkAllocationCallbacks* allocator,
			VkSurfaceKHR* surface) const;

		/**
		 * @returns the size in screen coordinates of the window
		 */
		UInt2 GetSize() const;

		/**
		 * @returns The size in pixels of the window
		 */
		UInt2 GetFramebufferSize() const;

		/**
		 * Terminates glfw
		 */
		~Window();

		/**
		 * Sleeps the current thread until new window messages arrive
		 */
		void WaitForEvents() const;

		void SetTitle(std::string_view newTitle) const;

		/**
		 * @returns The underlying GLFW window handle
		 */
		GLFWwindow* GetNative() { return m_hWnd; }

		/**
		 * @returns The RenderContext containing all vulkan rendering objects
		 */
		const RenderContext& GetRenderContext() const { return *m_RenderContext; }
		RenderContext& GetRenderContext() { return *m_RenderContext; }

		Ref<Scene> GetActiveScene() { return m_ActiveScene.lock(); }

		/**
		 * Sets the scene which is actively being rendered in this window
		 * RAY_TODO: Use WeakPtr<Scene> as argument to avoid atomic counter increment?
		 */
		void SetActiveScene(Ref<Scene> scene);

#if RAY_ENABLE_IMGUI
		ImGUI& GetImGui() { return *m_ImGui; }
#endif

	private:
		void SetEventCallbacks();

		void GenerateHoverEvents();
		Widget* GetClickedWidget();
		Widget* GetReleasedWidget();

		static void TryInitializeGlfw();

	private:
		static bool s_GlfwInitialized;

		Scope<RenderContext> m_RenderContext;
		WeakPtr<Scene> m_ActiveScene;

		GLFWwindow* m_hWnd = nullptr;
		Float2 m_PrevousMousePos{};
		Widget* m_HoverWidget = nullptr;
		Widget* m_ClickedWidget = nullptr;

#if RAY_ENABLE_IMGUI
		Scope<ImGUI> m_ImGui;
#endif
	};
}  // namespace At0::Ray
