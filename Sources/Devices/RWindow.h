#pragma once

#include "../RBase.h"
#include "../Utils/RMath.h"

#include <string_view>
#include <stdint.h>

struct GLFWwindow;


namespace At0::Ray
{
	class RAY_EXPORT Window
	{
	public:
		/**
		 * Creates the Window
		 */
		static Window& Create(uint32_t width = 960, uint32_t height = 540, std::string_view title = "Window");

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

	private:
		Window(uint32_t width, uint32_t height, std::string_view title);
		~Window();
		void SetEventCallbacks();

	private:
		static Window* s_Instance;
		GLFWwindow* m_hWnd = nullptr;
		Float2 m_CachedRawDeltaMousePos {};
	};
}  // namespace At0::Ray
