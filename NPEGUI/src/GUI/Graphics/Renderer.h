#pragma once

#include "GUIBase.h"
#include "Util/Util.h"
#include "Win.h"

#include <wrl.h>
#include <dwrite.h>


/**QUESTION:
*	Where should i put static global variables?
*/

namespace GUI
{
	static constexpr Util::NColor g_DefaultLineColor{ 160.0f, 160.0f, 160.0f };
	static constexpr Util::NColor g_DefaultTextColor{ 160.0f, 160.0f, 160.0f };
	static constexpr Util::NColor g_DefaultLineButtonColor{ 160.0f, 160.0f, 160.0f };
	static constexpr Util::NColor g_DefaultWindowBackgroundColor{ 35.0f, 38.0f, 40.0f };
	static constexpr Util::NColor g_DefaultNodeColor{ 15.0f, 17.0f, 19.0f };
	static constexpr Util::NColor g_DefaultTextBoxColor{ 35.0f, 38.0f, 40.0f };

	/**
	* Renderer class used to render the GUI
	*/
	class GUI_API Renderer
	{
		friend class TextRenderer;
	public:
		/**
		* Initializes the renderer
		* 
		* @param hWnd is the main window
		*/
		void Init(HWND hWnd);

		/**
		* Getter for static Renderer instance (Singelton design)
		*
		* @returns the renderer
		*/
		static Renderer& Get() { return *s_Instance; };

		/**
		* Wrapper for ID2D1HwndRenderTarget::BeginDraw function
		*/
		void BeginDraw();

		/**
		* Wrapper for ID2D1HwndRenderTarget::EndDraw function
		*/
		void EndDraw();
		
		/**
		* Renders the background with a color (currently renders bitmap, parameter color is ignored)
		*
		* @param color is the background color
		*/
		void RenderScene(const Util::NColor& color = g_DefaultWindowBackgroundColor);

		/**
		* Draws a filled rectangle with specified arguments
		* 
		* @param pos is the x and y coordinate of the rect
		* @param size is the width and height of the rect
		* @param color are the rgb values of the rect
		*/
		void RenderRect(const Util::NPoint& pos, const Util::NSize& size, const Util::NColor& color);

		/**
		* Draws a filled rounded rectangle with specified arguments
		*
		* @param pos is the x and y coordinate of the rounded rect
		* @param size is the width and height of the rounded rect
		* @param color are the rgb values of the rounded rect
		* @param radiusX is the x value of the rounded corner
		* @param radiusY is the y value of the rounded corner
		*/
		void RenderRoundedRect(const Util::NPoint& pos, const Util::NSize& size, const Util::NColor& color, const float radiusX, const float radiusY);

		/**
		* Draws a filled line from startPos to endPos
		* 
		* @param startPos is the start position of the line
		* @param endPos is the end position of the line
		*/
		void RenderLine(const Util::NPoint& startPos, const Util::NPoint& endPos, const Util::NColor& color = g_DefaultLineColor, const float radius = 1.0f);

		/**
		* Getter for main HWND
		* 
		* @returns the main window's hWnd
		*/
		HWND GetNativeWindow() { return m_hWnd; }

	private:
		/**
		* Private renderer constructor
		*/
		Renderer();

		/**
		* Creates all Graphics resources (member variables)
		*/
		void CreateGraphicsResources();

		/**
		* Uses the bitmap in relative directory (name = BackgroundImage.bmp) to draw the background
		*/
		void RenderBitmapBackground();
	private:
		HWND m_hWnd;

		Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;

		Microsoft::WRL::ComPtr<IDWriteFactory> m_pWriteFactory;

		Microsoft::WRL::ComPtr<ID2D1Bitmap> m_pD2DBitmap;

		static Renderer* s_Instance;
	};
}



