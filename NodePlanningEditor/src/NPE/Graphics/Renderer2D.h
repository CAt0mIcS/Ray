#pragma once

#include "NPE/Util/Props.h"

#include "NPE/Controls/Control.h"

#include "Win.h"

#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

#include <string>
#include <wrl.h>

/**QUESTION:
*	Where should i put static global variables? 
*/


namespace NPE
{
	static constexpr NColor g_DefaultLineColor{ 160.0f, 160.0f, 160.0f };
	static constexpr NColor g_DefaultTextColor{ 160.0f, 160.0f, 160.0f };
	static constexpr NColor g_DefaultLineButtonColor{ 160.0f, 160.0f, 160.0f };

	class Renderer2D
	{
	public:
		/**
		* Default Renderer2D contructor
		*/
		Renderer2D();

		/**
		* Initializes the renderer
		* 
		* @param hWnd is the main HWND
		* @param fontSize is the future font size (to be changed)
		*/
		void Init(HWND hWnd);

		/**
		* Wrapper for ID2D1HwndRenderTarget::BeginDraw function
		*/
		void BeginDraw();

		/**
		* Wrapper for ID2D1HwndRenderTarget::EndDraw function
		*/
		void EndDraw();

		/**
		* Uses the control's position, size and color to render it with round corners
		* 
		* @param control is the control to render
		*/
		void RenderRoundedRectControl(const Control& control, float radiusX = 0.0f, float radiusY = 0.0f) const;

		/**
		* Renders a round border
		*
		* @param control is the control to render
		*/
		void RenderRoundedRectBorder(const NPoint& pos, const NSize& size, const NColor& color, float radiusX = 0.0f, float radiusY = 0.0f) const;

		/**
		* Renders a text to the screen
		* 
		* @param text is the text to render
		* @param pos are the x and y coordinates of the text
		* @param size is the size of the text
		*/
		void RenderText(const std::wstring& text, const NPoint& pos, const NColor& color, const float fontSize);

		/**
		* Uses the bitmap in relative directory (name = BackgroundImage.bmp) to draw the background
		*/
		void RenderBitmapBackground();

		/**
		* Draws a line
		* 
		* @param startPos is the start position of the line
		* @param endPos is the end position of the line
		* @param color is the color of the line
		* @param width is the width of the line
		*/
		void RenderLine(const NPoint& startPos, const NPoint& endPos, const NColor& color, const unsigned int width);

		/**
		* Renders the background with a color
		* 
		* @param color is the background color
		*/
		void RenderScene(const NColor& color = { 35.0f, 38.0f,40.0f });

		/**
		* Getter for the HWND passed into the Init function
		* 
		* @returns the native window HWND
		*/
		const HWND GetNativeWindow() const { return m_hWnd; }

		/**
		* Uses ID2D1RoundedRectangleGeometry::CompareWithGeometry to identify if two controls overlapp
		* 
		* @param cOne is the first control
		* @param cTwo is the second control
		* @returns true if the controls overlap, false otherwise
		*/
		bool RoundedRectConrolsOverlap(const Control& cOne, const Control& cTwo, const NSize& minDst = { 0.0f, 0.0f });

	private:
		/**
		* Initializes all private ComPtr variables, function is called in Renderer2D::Init
		* 
		* @param fontSize is the fontSize passed in by Init
		*/
		void CreateGraphicsResources();

	private:
		HWND m_hWnd;

		Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush > m_pBrush;
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
		
		//for drawing text
		Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
		Microsoft::WRL::ComPtr<IDWriteFactory> m_pWriteFactory;

		//for drawing background bitmap
		Microsoft::WRL::ComPtr<ID2D1Bitmap> m_pD2DBitmap;
	};
}



