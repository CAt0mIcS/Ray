#include "pch.h"
#include "TextRenderer.h"

#include "Util/Exceptions.h"


namespace GUI
{
	TextRenderer* TextRenderer::s_TextRenderer = new TextRenderer();

	void TextRenderer::Init()
	{
		CreateTextGraphicsResources();
	}

	void TextRenderer::RenderText(const NText& text)
	{
		IDWriteTextFormat* pFormat;
		NPE_THROW_GFX_EXCEPT(m_pFactory->CreateTextFormat(text.fontFamily.c_str(),
			nullptr, text.fontWeight, text.fontStyle, text.fontStretch, text.fontSize,
			text.localeName.c_str(), &pFormat), "Failed to create IDWriteTextFormat"
		);

		IDWriteTextLayout* pLayout;
		NPE_THROW_GFX_EXCEPT(m_pFactory->CreateTextLayout(text.text.c_str(),
			wcslen(text.text.c_str()), pFormat, text.size.width, text.size.height, &pLayout),
			"Failed to create DWriteTextLayout"
		);

		Renderer::Get().m_pBrush->SetColor(text.color.ToD2D1ColorF());

		Renderer::Get().m_pRenderTarget->DrawTextW(
			text.text.c_str(), (UINT32)wcslen(text.text.c_str()), pFormat,
			{ text.pos.x, text.pos.y, text.pos.x + text.size.width , text.pos.y + text.size.height },
			Renderer::Get().m_pBrush.Get(), text.options, text.measuringMode
		);


		DWRITE_HIT_TEST_METRICS hitTestMetrics;
		float caretX, caretY;
		bool isTrailingHit = false; // Use the leading character edge for simplicity here.

		// Map text position index to caret coordinate and hit-test rectangle.
		NPE_THROW_GFX_EXCEPT(pLayout->HitTestTextPosition(
			text.pos.x,
			isTrailingHit,
			OUT & caretX,
			OUT & caretY,
			OUT & hitTestMetrics
		), "Failed hit test text position");

		if (hitTestMetrics.textPosition < 11)
			__debugbreak();

		// Respect user settings.
		DWORD caretWidth = 1;
		SystemParametersInfo(SPI_GETCARETWIDTH, 0, OUT & caretWidth, 0);
		DWORD halfCaretWidth = caretWidth / 2u;

		// Draw a thin rectangle.
		D2D1_RECT_F caretRect = {
			text.pos.x + caretX - halfCaretWidth,
			text.pos.y + hitTestMetrics.top,
			text.pos.x + caretX + (caretWidth - halfCaretWidth),
			text.pos.y + hitTestMetrics.top + hitTestMetrics.height
		};
		Renderer::Get().m_pBrush->SetColor({ 255, 255, 255 });
		Renderer::Get().m_pRenderTarget->FillRectangle(&caretRect, Renderer::Get().m_pBrush.Get());

		pLayout->Release();
		pFormat->Release();
	}

	TextRenderer::TextRenderer()
	{
	}

	void TextRenderer::CreateTextGraphicsResources()
	{
		NPE_THROW_GFX_EXCEPT(DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, 
			__uuidof(m_pFactory), &m_pFactory), "Failed to create IDWriteFactory");
	}
}


