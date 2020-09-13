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
			(UINT32)wcslen(text.text.c_str()), pFormat, text.size.width, text.size.height, &pLayout),
			"Failed to create DWriteTextLayout"
		);

		DWRITE_TEXT_METRICS metrics;
		NPE_THROW_GFX_EXCEPT(pLayout->GetMetrics(&metrics), "Failed to get text metrics");

		unsigned int lineCountThreshold = 0;

		if (metrics.lineCount > lineCountThreshold)
		{

		}


		Renderer::Get().m_pBrush->SetColor(text.color.ToD2D1ColorF());
		Renderer::Get().m_pRenderTarget->DrawTextW(
			text.text.c_str(), (UINT32)wcslen(text.text.c_str()), pFormat,
			{ text.pos.x, text.pos.y, text.pos.x + text.size.width , text.pos.y + text.size.height },
			Renderer::Get().m_pBrush.Get(), text.options, text.measuringMode
		);

		pFormat->Release();
		pLayout->Release();
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


