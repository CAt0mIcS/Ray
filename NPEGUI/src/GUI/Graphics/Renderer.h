#pragma once

#include "Win.h"
#include "GUIBase.h"
#include "Util/Util.h"

#include <wrl.h>
#include <dwrite.h>

#include "Util/Constants.h"
#include "Util/Direct2D.h"


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
		void RenderScene();

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
			_In_ const Direct2D::RectF& rect, 
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

		/// <summary>
		/// Calculates the view matrix
		/// </summary>
		/// <param name="origin">Is the point where the window should zoom towards</param>
		/// <returns>The view matrix</returns>
		DWrite::Matrix GetViewMatrix(_In_ const Util::NPoint& origin);

		/// <summary>
		/// Calculates the inverse view matrix
		/// </summary>
		/// <param name="origin">Is the point where the window should zoom towards</param>
		/// <returns>The inverse view matrix</returns>
		DWrite::Matrix GetInverseViewMatrix(_In_ const Util::NPoint& origin);

		/// <summary>
		/// Calculates the inverse matrix from a normal matrix
		/// </summary>
		/// <param name="matrix">Is the normal matrix</param>
		/// <returns>The inverse matrix</returns>
		DWrite::Matrix ComputeInverseMatrix(_In_ const DWrite::Matrix& matrix);

		/// <summary>
		/// Getter for the current transform of the MainWindow
		/// </summary>
		/// <returns>The current window transform</returns>
		DWrite::Matrix GetTransform();

		/// <summary>
		/// Setter for the MainWindow transform
		/// </summary>
		/// <param name="transform">Is the new window transform</param>
		void SetTransform(_In_ const DWrite::Matrix& transform);

		/// <summary>
		/// Getter for the window scale
		/// </summary>
		/// <returns>The current window scale</returns>
		const Util::NSize& GetScale() const { return m_Scale; }

		/// <summary>
		/// Setter for the window scale
		/// </summary>
		/// <param name="scale">Is the new scale of the window</param>
		void SetScale(const Util::NSize& scale) { m_Scale = scale; }

		/// <summary>
		/// Caclulates the coordinates of the middle of the screen
		/// </summary>
		/// <returns>The origin of the screen</returns>
		Util::NPoint GetOrigin() const;

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

	private:
		/// <summary>
		/// Handle to the main window
		/// </summary>
		HWND m_hWnd;

		/// <summary>
		/// Represents the scale of the current window
		/// </summary>
		Util::NSize m_Scale;

		/// <summary>
		/// Static Renderer instance for singelton design
		/// </summary>
		static Renderer* s_Instance;
	};
}



