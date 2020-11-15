#ifndef RL_LINUXBASEWINDOW_H
#define RL_LINUXBASEWINDOW_H

#include <../../RlUtilities/include/RlUtil/PlatformDetection.h>

#ifdef RL_PLATFORM_LINUX

#include <stdint.h>
#include <../../extern/glfw/include/GLFW/glfw3.h>


namespace At0::Reyal
{

	struct WindowMessage
	{

	};


	template<typename DERIVED_TYPE>
	class BaseWindow
	{
	public:
		/// <summary>
		/// BaseWindow Constructor
		/// </summary>
		BaseWindow()
			: m_hWnd(0) {}

		/// <summary>
		/// Getter for the current Window Title
		/// </summary>
		/// <returns>The title of this Window</returns>
		std::wstring GetTitle() const
		{
			// TODO: Throw unsuported
			return "";
		}

		/// <summary>
		/// Sets the Window Title
		/// </summary>
		/// <param name="title">Is the new Window Title</param>
		void SetTitle(const std::wstring_view title)
		{
			glfwSetWindowTitle(m_hWnd, title.data());
		}

		/// <summary>
		/// Shows the Window
		/// </summary>
		void Show() const
		{
			glfwShowWindow(m_hWnd);
		}

		/// <summary>
		/// Hides the Window
		/// </summary>
		void Hide() const
		{
			glfwHideWindow(m_hWnd);
		}

		/// <summary>
		/// Maximizes the Window
		/// </summary>
		void Maximize() const
		{
			glfwMaximizeWindow(m_hWnd);
		}

		/// <summary>
		/// Minimizes the Window
		/// </summary>
		void Minimize() const
		{
			glfwIconifyWindow(m_hWnd);
		}

		/// <summary>
		/// Closes the Window
		/// </summary>
		void Close() const
		{
			glfwDestroyWindow(m_hWnd);
		}

		/// <summary>
		/// Checks if the Window is open (IsVisible)
		/// </summary>
		/// <returns>True if the Window is open, false otherwise</returns>
		bool IsOpen() const
		{
			return true;
		}

		/// <summary>
		/// Virtual BaseWindow Deconstructor, unregisters the window class
		/// </summary>
		virtual ~BaseWindow()
		{

		}

		/// <summary>
		/// Creates the native window
		/// </summary>
		/// <param name="windowName">Is the title of the window</param>
		/// <param name="windowClassName">Is the window class name</param>
		/// <param name="style">Are window styles</param>
		/// <param name="exStyle">Are extended window styles</param>
		/// <param name="x">Is the x-position of the window</param>
		/// <param name="y">Is the y-position of the window</param>
		/// <param name="width">Is the width of the window</param>
		/// <param name="height">Is the height of the window</param>
		/// <param name="hWndParent">Is a handle to the parent window</param>
		/// <param name="hMenu">Is a window Id</param>
		/// <returns>TRUE(1) if the window was created successfully, FALSE(0) otherwise</returns>
		uint8_t CreateNativeWindow(
			const char* windowName,
			uint32_t x,
			uint32_t y,
			uint32_t width,
			uint32_t height,
		)
		{
			m_hWnd = glfwCreateWindow(width, height, windowName, nullptr, nullptr);
		}

	protected:
		virtual int64_t HandleMessage(const WindowMessage& msg) = 0;

	protected:
		::GLFWwindow* m_hWnd;
	};
}

#endif // RL_PLATFORM_LINUX


#endif // RL_LINUXBASEWINDOW_H