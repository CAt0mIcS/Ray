#pragma once

#include "Win.h"
#include "GUIBase.h"
#include "Util/Util.h"

#include <wrl.h>
#include <dwrite.h>

#include "Util/Constants.h"


namespace GUI
{
	/// <summary>
	/// Renderer class used to Render the GUI
	/// </summary>
	class GUI_API Renderer
	{
		friend class TextRenderer;
	public:
		/// <summary>
		/// Initializes the Renderer
		/// </summary>
		/// <param name="hWnd">Is a handle to the main window</param>
		void Init(_In_ HWND hWnd);

		/// <summary>
		/// Getter for static Renderer instance (Singelton design)
		/// </summary>
		/// <returns>The Renderer</returns>
		static Renderer& Get() { return *s_Instance; };

		/// <summary>
		/// Wrapper for ID2D1HwndRenderTarget::BeginDraw function
		/// </summary>
		void BeginDraw();

		/// <summary>
		/// Wrapper for ID2D1HwndRenderTarget::EndDraw function
		/// </summary>
		void EndDraw();


		/// <summary>
		/// Renders the background with a color (currently renders bitmap, parameter color is ignored)
		/// </summary>
		/// <param name="color">Is the background color</param>
		void RenderScene(
			_In_opt_ const Util::NColor& color = Constants::g_DefaultWindowBackgroundColor
		);

		/// <summary>
		/// Draws a filled rectangle with specified arguments
		/// </summary>
		/// <param name="pos">Is the x and y coordinate of the rect</param>
		/// <param name="size">Is the width and height of the rect</param>
		/// <param name="color">Are the rgb values of the rect</param>
		void RenderRect(
			_In_ const Util::NPoint& pos, 
			_In_ const Util::NSize& size, 
			_In_ const Util::NColor& color
		);

		/// <summary>
		/// Draws a filled rectangle with specified arguments
		/// </summary>
		/// <param name="rect">Is the rect of the rectangle to draw</param>
		/// <param name="color">Are the rgb values of the rect</param>
		void RenderRect(
			_In_ const D2D1_RECT_F& rect, 
			_In_ const Util::NColor& color
		);

		/// <summary>
		/// Draws a filled rounded rectangle with specified arguments
		/// </summary>
		/// <param name="pos">Is the x and y coordinate of the rounded rect</param>
		/// <param name="size">Is the width and height of the rounded rect</param>
		/// <param name="color">Are the rgb values of the rounded rect</param>
		/// <param name="radiusX">Is the x value of the rounded corner</param>
		/// <param name="radiusY">Is the y value of the rounded corner</param>
		void RenderRoundedRect(_In_ const Util::NPoint& pos, 
			_In_ const Util::NSize& size, 
			_In_ const Util::NColor& color, 
			_In_ const float radiusX, 
			_In_ const float radiusY
		);

		/// <summary>
		/// Draws a filled line from startPos to endPos
		/// </summary>
		/// <param name="startPos">Is the start position of the line</param>
		/// <param name="endPos">Is the end position of the line</param>
		/// <param name="color">Is the color of the line</param>
		/// <param name="radius">Is the radius of the line</param>
		void RenderLine(_In_ const Util::NPoint& startPos, 
			_In_ const Util::NPoint& endPos, 
			_In_opt_ const Util::NColor& color = Constants::g_DefaultLineColor, 
			_In_opt_ const float radius = 1.0f
		);

		/// <summary>
		/// Getter for main window handle
		/// </summary>
		/// <returns>A handle to the main window</returns>
		HWND GetNativeWindow() { return m_hWnd; }

	private:
		/// <summary>
		/// Renderer Constructor
		/// </summary>
		Renderer();

		/// <summary>
		/// Creates all graphics resources
		/// </summary>
		void CreateGraphicsResources();

		/// <summary>
		/// Uses the bitmap in the resource to draw the background
		/// </summary>
		void RenderBitmapBackground();
	private:
		/// <summary>
		/// Handle to the main window
		/// </summary>
		HWND m_hWnd;

		/// <summary>
		/// Factory object, used to create all other D2D1 objects
		/// </summary>
		Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;

		/// <summary>
		/// Render target, uses Handle to the main window
		/// </summary>
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;

		/// <summary>
		/// D2D1 Brush
		/// </summary>
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;

		/// <summary>
		/// Cached bitmap for background, access read violation when trying to release, thus is left unreleased
		/// </summary>
		ID2D1Bitmap* m_pD2DBitmap;

		/// <summary>
		/// Static Renderer instance for singelton design
		/// </summary>
		static Renderer* s_Instance;
	};
}



