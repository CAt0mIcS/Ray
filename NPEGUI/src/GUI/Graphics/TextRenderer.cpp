#include "pch.h"
#include "TextRenderer.h"

#include "Util/Exceptions.h"
#include "Util/Direct2D.h"

#include "GUI/Handlers/Mouse.h"


namespace GUI
{
	TextRenderer* TextRenderer::s_TextRenderer = new TextRenderer();

	void TextRenderer::Init()
	{
		CreateTextGraphicsResources();
	}

	DWRITE_TEXT_METRICS TextRenderer::GetTextMetrics(const NText& text)
	{
		//TODO: Add exception which will be thrown if the user didn't call CreateTextLayout
		if (!m_pLayout)
			return {};

		DWRITE_TEXT_METRICS metrics;
		CreateTextLayout(text);

		NPE_THROW_GFX_EXCEPT(m_pLayout->GetMetrics(&metrics), "Failed to get text metrics");

		return metrics;
	}

	void TextRenderer::RenderText(const NText& text)
	{
		//TODO: Add exception which will be thrown if the user didn't call CreateTextLayout
		if (!m_pLayout)
			return;

		Renderer::Get().m_pBrush->SetColor(text.color.ToD2D1ColorF());
		Renderer::Get().m_pRenderTarget->DrawTextLayout(text.pos.ToD2D1Point2F(), m_pLayout.Get(), Renderer::Get().m_pBrush.Get());
	}

	DWRITE_HIT_TEST_METRICS TextRenderer::HitTestPoint(const NText& text, BOOL* isTrailingHit, BOOL* isInside)
	{
		//TODO: Add exception which will be thrown if the user didn't call CreateTextLayout
		if (!m_pLayout)
			return{};

		DWRITE_HIT_TEST_METRICS metrics;

		float transformedX = Mouse::GetPos().x - text.pos.x;
		float transformedY = Mouse::GetPos().y - text.pos.y;

		NPE_THROW_GFX_EXCEPT(m_pLayout->HitTestPoint(
			transformedX,
			transformedY,
			isTrailingHit,
			isInside,
			&metrics),
			"Failed to run DWriteTextLayout::HitTestPoint"
		);
		return metrics;
	}

	TextRenderer::TextRenderer()
	{
	}

	void TextRenderer::CreateTextGraphicsResources()
	{
		NPE_THROW_GFX_EXCEPT(DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, 
			__uuidof(m_pFactory), &m_pFactory), "Failed to create IDWriteFactory");
	}
	
	void TextRenderer::CreateTextLayout(const NText& text)
	{
		IDWriteTextFormat* pFormat;
		NPE_THROW_GFX_EXCEPT(m_pFactory->CreateTextFormat(text.fontFamily.c_str(),
			nullptr, text.fontWeight, text.fontStyle, text.fontStretch, text.fontSize,
			text.localeName.c_str(), &pFormat), "Failed to create IDWriteTextFormat"
		);

		NPE_THROW_GFX_EXCEPT(m_pFactory->CreateTextLayout(
			text.text.c_str(), (UINT32)wcslen(text.text.c_str()), pFormat,
			text.size.width, text.size.height, &m_pLayout), 
			"Failed to create DWriteTextLayout"
		);

		Util::Release(&pFormat);
	}
}


