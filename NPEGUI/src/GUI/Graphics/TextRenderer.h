#pragma once

#include "GUIBase.h"
#include "Renderer.h"

#include <dwrite.h>
#include <wrl.h>



namespace GUI
{
	struct GUI_API NText
	{
		std::wstring text = L"";
		Util::NPoint pos = { 0.0f, 0.0f, };
		Util::NSize size = { 0.0f, 0.0f };
		Util::NColor color = { 255.0f, 255.0f, 255.0f };
		FLOAT fontSize = 20;
		std::wstring fontFamily = L"Consolas";
		std::wstring localeName = L"";
		DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL;
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL;
		D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE;
		DWRITE_MEASURING_MODE measuringMode = DWRITE_MEASURING_MODE_NATURAL;
	};

	class GUI_API TextRenderer
	{
	public:
		/**
		* Getter for static TextRenderer (singelton design)
		* 
		* @returns the TextRenderer
		*/
		static TextRenderer& Get() { return *s_TextRenderer; }
		
		/**
		* Initializes the TextRenderer
		*/
		void Init();

		/**
		* Returns the text metrics to the current text
		* 
		* @param text is the text to get the metrics for
		* @returns the text metrics to text
		*/
		DWRITE_TEXT_METRICS GetTextMetrics(const NText& text);

		/**
		* Renders text to specifies bounding rect
		* 
		* @param text is the NText structure containing all text info
		* @see GUI::NText;
		*/
		void RenderText(const NText& text);

	private:
		TextRenderer();
		void CreateTextGraphicsResources();

	private:
		Microsoft::WRL::ComPtr<IDWriteFactory> m_pFactory;

		static TextRenderer* s_TextRenderer;
	};
}



