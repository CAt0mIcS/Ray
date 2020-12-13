#pragma once

#include "../RBase.h"

#ifdef _WIN32

#include "RWindow.h"

#include <Windows.h>

#include <../../RayRender/include/RayRender/TypeDefines.h>


namespace At0::Ray
{
	class Renderer3D;
	//class Renderer2D;

	class RAY_API WinAPIWindow : public Window
	{
	public:
		/// <summary>
		/// Window Constructor
		/// </summary>
		/// <param name="name">Is the unique name of the window</param>
		/// <param name="parent">Is the parent of this window</param>
		WinAPIWindow(const std::string_view name, const Point2 pos, const Size2 size, Widget* parent = nullptr);
		
		/// <summary>
		/// Window Deconstructor
		/// </summary>
		~WinAPIWindow();

		/// <summary>
		/// Getter for the current Window Title
		/// </summary>
		/// <returns>The title of this Window</returns>
		virtual std::string GetTitle() const override;

		/// <summary>
		/// Sets the Window Title
		/// </summary>
		/// <param name="title">Is the new Window Title</param>
		virtual void SetTitle(const std::string_view title) override;

		/// <summary>
		/// Shows the Window
		/// </summary>
		virtual void Show() const override;

		/// <summary>
		/// Hides the Window
		/// </summary>
		virtual void Hide() const override;

		/// <summary>
		/// Maximizes the Window
		/// </summary>
		void Maximize() const override;

		/// <summary>
		/// Minimizes the Window
		/// </summary>
		virtual void Minimize() const override;

		/// <summary>
		/// Closes the Window
		/// </summary>
		virtual void Close() override;

		/// <summary>
		/// Checks if the Window is open (IsVisible)
		/// </summary>
		/// <returns>True if the Window is open, false otherwise</returns>
		virtual bool IsOpen() const override;

		/// <summary>
		/// Called every frame to read from the internal message queue
		/// </summary>
		virtual void Update() override;

		/// <summary>
		/// Sets the window icon
		/// </summary>
		/// <param name="path">Is the path to the .ico file</param>
		virtual void SetIcon(const std::string_view path) override;

		/// <summary>
		/// Getter for the Native Window
		/// </summary>
		/// <returns>The Native Window</returns>
		virtual void* GetNativeWindow() const override { return (void*)m_hWnd; }
		
		/// <summary>
		/// Initializes the 3D Renderer, checks if it has already been initialized
		/// </summary>
		virtual bool InitRenderer3D() override;

		/// <summary>
		/// Initializes the 2D Renderer, checks if it has already been initialized
		/// </summary>
		virtual bool InitRenderer2D() override;

		/// <summary>
		/// Getter for the Renderer of this Window
		/// </summary>
		/// <returns>The Window Renderer</returns>
		virtual Renderer3D* GetRenderer3D() const override;

	private:
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
		/// <returns>true if the window was created successfully, false otherwise</returns>
		bool CreateNativeWindow(
			const wchar_t* windowName,
			const wchar_t* windowClassName,
			DWORD style,
			DWORD exStyle = 0,
			uint32_t x = CW_USEDEFAULT,
			uint32_t y = CW_USEDEFAULT,
			uint32_t width = CW_USEDEFAULT,
			uint32_t height = CW_USEDEFAULT,
			HWND hWndParent = 0,
			HMENU hMenu = 0
		);

		static LRESULT WindowProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT WindowProcPass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		/// <summary>
		/// Handles all window messages
		/// </summary>
		/// <param name="uMsg">Is the message code</param>
		/// <param name="wParam">Is an additional parameter</param>
		/// <param name="lParam">Is an additional parameter</param>
		/// <returns>LRESULT code</returns>
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		bool m_IsOpen;

		/// <summary>
		/// The native window handle
		/// </summary>
		HWND m_hWnd;

		/// <summary>
		/// Specifies the 3D renderer
		/// </summary>
		Scope<Renderer3D> m_Renderer3D;

		///// <summary>
		///// Specifies the 2D renderer
		///// </summary>
		//Scope<Renderer2D> m_Renderer2D;

		/// <summary>
		/// Specifies the old size of this window
		/// </summary>
		Size2 m_OldSize;

		/// <summary>
		/// Specifies the old position of this window
		/// </summary>
		Point2 m_OldPos;
	};
}

#endif // _WIN32


