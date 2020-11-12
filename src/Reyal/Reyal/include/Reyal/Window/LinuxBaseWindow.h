#ifndef RL_LINUXBASEWINDOW_H
#define RL_LINUXBASEWINDOW_H

#include <../../RlUtilities/include/RlUtil/PlatformDetection.h>

#ifdef RL_PLATFORM_LINUX

#include <X11/Xlib.h>
#include <stdint.h>


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
			return L"";
		}

		/// <summary>
		/// Sets the Window Title
		/// </summary>
		/// <param name="title">Is the new Window Title</param>
		void SetTitle(const std::wstring_view title)
		{
			
		}

		/// <summary>
		/// Shows the Window
		/// </summary>
		void Show() const
		{
			
		}

		/// <summary>
		/// Hides the Window
		/// </summary>
		void Hide() const
		{
			
		}

		/// <summary>
		/// Maximizes the Window
		/// </summary>
		void Maximize() const
		{
			
		}

		/// <summary>
		/// Minimizes the Window
		/// </summary>
		void Minimize() const
		{
			
		}

		/// <summary>
		/// Closes the Window
		/// </summary>
		void Close() const
		{
			
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


	protected:
		virtual int64_t HandleMessage(const WindowMessage& msg) = 0;

	protected:
		::Window* m_hWnd;
	};
}

#endif // RL_PLATFORM_LINUX


#endif // RL_LINUXBASEWINDOW_H