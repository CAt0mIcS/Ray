#pragma once

#include "../RBase.h"

#ifdef _WIN32

#include "RWindow.h"

#include <Windows.h>

#include <../../RayUtil/include/RayUtil/GlobalDefines.h>


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
		WinAPIWindow(std::string_view name, const Point2 pos, const Size2 size, Widget* parent = nullptr);
		~WinAPIWindow();

		virtual std::string GetTitle() const override;
		virtual void SetTitle(std::string_view title) override;

		virtual void Show() const override;
		virtual void Hide() const override;
		virtual void Maximize() const override;
		virtual void Minimize() const override;

		/// <summary>
		/// Moves the window to screen coordinates in pos
		/// </summary>
		/// <param name="pos">Are the coordinates where the window will be moved to</param>
		virtual void SetPos(const Point2& pos) override;

		/// <summary>
		/// Resizes the window
		/// </summary>
		/// <param name="size">Is the new size of the window</param>
		virtual void SetSize(const Size2& size) override;

		/// <summary>
		/// Getter for the current window position
		/// </summary>
		/// <returns>The window position</returns>
		virtual Point2 GetPos() const override;

		/// <summary>
		/// Getter for the current window size
		/// </summary>
		/// <returns>The current window size</returns>
		virtual Size2 GetSize() const override;

		virtual void Close() override;
		virtual bool IsOpen() const override;

		/// <summary>
		/// Called every frame to read from the internal message queue
		/// </summary>
		virtual void Update() override;

		/// <summary>
		/// Sets the window icon
		/// </summary>
		/// <param name="path">Is the path to the .ico file</param>
		virtual void SetIcon(std::string_view path) override;

	private:
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
		HWND m_hWnd;

		// Represents data that needs to be destroyed when the window closes
		struct
		{
			HICON hCurrIcon;
		} m_DestroyOnWindowCloseData{};
	};
}

#endif // _WIN32


