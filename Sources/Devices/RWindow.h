#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"

#include <string_view>
#include <utility>
#include <stdint.h>

struct GLFWwindow;
struct VkInstance_T;
struct VkAllocationCallbacks;
struct VkSurfaceKHR_T;

namespace At0::Ray
{
	class RAY_EXPORT Window
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

		/*
		 * Polls events and updates everything
		 * @returns If the window should stay open
		 */
		bool Update();

		/**
		 * Querys required extensions for Vulkan
		 * @returns All extensions that the Vulkan instance needs
		 */
		std::pair<const char**, uint32_t> GetInstanceExtensions() const;

		/**
		 * Creates the surface to render to
		 */
		void CreateSurface(VkInstance_T* instance, const VkAllocationCallbacks* allocator,
			VkSurfaceKHR_T** surface) const;

		/**
		 * @returns the size in pixels of the window
		 */
		UInt2 GetSize() const;

		/**
		 * Closes the window if it's still open and terminates glfw
		 */
		~Window();

	private:
		Window(uint32_t width, uint32_t height, std::string_view title);
		void SetEventCallbacks();

	private:
		static Scope<Window> s_Instance;
		GLFWwindow* m_hWnd = nullptr;
		Float2 m_CachedRawDeltaMousePos{};
	};
}  // namespace At0::Ray
