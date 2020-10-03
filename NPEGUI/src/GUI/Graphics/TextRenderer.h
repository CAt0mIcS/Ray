#pragma once

#include "GUIBase.h"
#include "Renderer.h"



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
		DWRITE_TEXT_METRICS GetTextMetrics(
			_In_ const NText& text
		);

		/**
		* Getter for the line metrics to the current text
		* 
		* @param text is the text to get the metrics for 
		* @returns the line metrics to the current text
		*/
		std::vector<DWRITE_LINE_METRICS> GetLineMetrics(
			_In_ const NText& text
		);

		/**
		* Getter for cluster metrics in current text
		* 
		* @param text is the text to get the metrics for
		* @param clusterCount will be filled with the amount of clusters
		* @returns the cluster metrics in current text
		*/
		std::vector<DWRITE_CLUSTER_METRICS> GetClusterMetrics(
			_In_ const NText& text, 
			_Out_ unsigned int* clusterCount
		);

		/**
		* Renders text to specifies bounding rect
		* 
		* @param text is the NText structure containing all text info
		* @see GUI::NText;
		*/
		void RenderText(
			_In_ const NText& text
		);

		/**
		* Creates IDWriteTextLayout and calls its HitTestPoint function
		* 
		* @param text is the current text
		* @param isTrailingHit will be filled by IDWriteTextLayout::HitTestPoint
		* @param isInside will be filled by IDWRiteTextLayout::HitTestPoint
		* 
		* @returns the hit test metrics
		*/
		DWRITE_HIT_TEST_METRICS HitTestPoint(
			_In_ const NText& text, 
			_Out_ BOOL* isTrailingHit, 
			_Out_ BOOL* isInside
		);
		
		/**
		* Creates IDWriteTextLayout and calls its HitTestTextPosition function
		*
		* @param text is the current text
		* @param textPos is the text position to look for
		* @param isTrailling hit is received by HitTestPoint
		* @param caretX is the x output position
		* @param caretY is the y output position
		* @returns the hit test text metrics
		*/
		DWRITE_HIT_TEST_METRICS HitTestTextPosition(
			_In_ const NText& text, 
			_In_ unsigned int textPos, 
			_In_ BOOL isTrailingHit, 
			_Out_ float* caretX, 
			_Out_ float* caretY
		);
		
		/**
		* Creates a IDWriteTextLayout with the text
		* 
		* @param text is the text of the new layout
		* @paaram ppLayout will be filled with the new layout
		*/
		void CreateTextLayout(
			_In_ const NText& text, 
			_Out_ IDWriteTextLayout** ppLayout
		);

	private:
		/**
		* Private TextRenderer constructor (Singelton design)
		*/
		TextRenderer();

		/**
		* Initializes m_pFactory
		*/
		void CreateTextGraphicsResources();


	private:
		Microsoft::WRL::ComPtr<IDWriteFactory> m_pFactory;

		static TextRenderer* s_TextRenderer;
	};
}



