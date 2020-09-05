#pragma once

#include "NPE/Util/Props.h"

#include "NPE/Controls/Control.h"

#include "Win.h"

#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

#include <string>
#include <wrl.h>


namespace NPE
{
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
		void Init(HWND hWnd, const unsigned int fontSize);

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
		void RenderRoundedRectControl(const Control& control) const;

		/**
		* Renders a text to the screen
		* 
		* @param text is the text to render
		* @param pos are the x and y coordinates of the text
		* @param size is the size of the text
		*/
		void RenderText(const std::string text, const NPoint& pos, const NSize& size);

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
		void RenderScene(const NColor& color);

		/**
		* Getter for the HWND passed into the Init function
		* 
		* @returns the native window HWND
		*/
		const HWND GetNativeWindow() const { return m_hWnd; }

	private:
		/**
		* Initializes all private ComPtr variables, function is called in Renderer2D::Init
		* 
		* @param fontSize is the fontSize passed in by Init
		*/
		void CreateGraphicsResources(const unsigned int fontSize);

	private:
		HWND m_hWnd;

		Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush > m_pBrush;
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pTextFormat;
		Microsoft::WRL::ComPtr<IDWriteFactory> m_pWriteFactory;
	};
}



