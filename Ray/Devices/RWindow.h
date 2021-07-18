#pragma once

#include "Ray/RBase.h"
#include "Ray/Core/RMath.h"

#include "Ray/Events/REventDispatcher.h"
#include "Ray/Events/REngineEvents.h"
#include "Ray/Events/RMouseEvents.h"
#include "Ray/Events/RKeyboardEvents.h"

#include <string_view>
#include <utility>
#include <stdint.h>

#include <vulkan/vulkan_core.h>
#include <RayRenderer/Core/RSurface.h>

struct GLFWwindow;


namespace At0::Ray
{
	class Widget;

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
		 * Creates the Window
		 */
		static Window& Create(
			uint32_t width = 960, uint32_t height = 540, std::string_view title = "Window");

		/**
		 * Getter for the static window instance.
		 * Window must be created before calling this function.
		 */
		static Window& Get();

		/**
		 * Shows the window
		 */
		void Show() const;

		/**
		 * Closes the window
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
		 * Polls events and updates everything
		 * @returns If the window should stay open
		 */
		bool Update();

		/**
		 * Querys required extensions for Vulkan
		 * @returns All extensions that the Vulkan instance needs
		 */
		static std::pair<const char**, uint32_t> GetInstanceExtensions();

		/**
		 * Creates the surface to render to
		 */
		void CreateSurface(RrSurfaceKHR* pSurface) const;

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
		 * @returns Internal, platform-dependent window handle
		 */
		void* GetInternalHandle() const;

	private:
		Window(uint32_t width, uint32_t height, std::string_view title);
		void SetEventCallbacks();

		void GenerateHoverEvents();
		Widget* GetClickedWidget();
		Widget* GetReleasedWidget();

	private:
		static Scope<Window> s_Instance;

		GLFWwindow* m_hWnd = nullptr;
		Float2 m_PrevousMousePos{};
		Widget* m_HoverWidget = nullptr;
		Widget* m_ClickedWidget = nullptr;
	};
}  // namespace At0::Ray
