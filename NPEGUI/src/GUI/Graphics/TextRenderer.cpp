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

	DWRITE_TEXT_METRICS TextRenderer::GetTextMetrics(_In_ const NText& text)
	{
		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);

		DWRITE_TEXT_METRICS metrics;

		NPE_THROW_GFX_EXCEPT(pLayout->GetMetrics(&metrics), "Failed to get text metrics");
		Util::Release(&pLayout);

		return metrics;
	}

	std::vector<DWRITE_LINE_METRICS> TextRenderer::GetLineMetrics(_In_ const NText& text)
	{
		std::vector<DWRITE_LINE_METRICS> lineMetrics;

		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);

		DWRITE_TEXT_METRICS metrics = GetTextMetrics(text);
		lineMetrics.resize(metrics.lineCount);

		pLayout->GetLineMetrics(&lineMetrics.front(), metrics.lineCount, &metrics.lineCount);
		Util::Release(&pLayout);
		return lineMetrics;
	}

	std::vector<DWRITE_CLUSTER_METRICS> TextRenderer::GetClusterMetrics(
		_In_ const NText& text, 
		_Out_ unsigned int* clusterCount
		)
	{
		std::vector<DWRITE_CLUSTER_METRICS> clusterMetrics;

		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);

		pLayout->GetClusterMetrics(nullptr, 0, clusterCount);
		if (clusterCount == 0)
			return {};

		clusterMetrics.resize(*clusterCount);
		pLayout->GetClusterMetrics(&clusterMetrics.front(), *clusterCount, clusterCount);

		Util::Release(&pLayout);
		return clusterMetrics;
	}

	void TextRenderer::RenderText(_In_ const NText& text)
	{
		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);

		Renderer2D::Get().m_pBrush->SetColor(text.color.ToD2D1ColorF());
		Renderer2D::Get().m_pRenderTarget->DrawTextLayout(text.pos.ToD2D1Point2F(), pLayout, Renderer2D::Get().m_pBrush.Get());
		Util::Release(&pLayout);
	}

	DWRITE_HIT_TEST_METRICS TextRenderer::HitTestPoint(
		_In_ const NText& text, 
		_Out_ BOOL* isTrailingHit, 
		_Out_ BOOL* isInside
		)
	{
		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);

		DWRITE_HIT_TEST_METRICS metrics;

		float transformedX = Mouse::GetPos().x - text.pos.x;
		float transformedY = Mouse::GetPos().y - text.pos.y;

		NPE_THROW_GFX_EXCEPT(pLayout->HitTestPoint(
			transformedX,
			transformedY,
			isTrailingHit,
			isInside,
			&metrics),
			"Failed to run DWriteTextLayout::HitTestPoint"
		);

		Util::Release(&pLayout);
		return metrics;
	}

	DWRITE_HIT_TEST_METRICS TextRenderer::HitTestTextPosition(
		_In_ const NText& text, 
		_In_ unsigned int textPos, 
		_In_ BOOL isTrailingHit, 
		_Out_ float* caretX, 
		_Out_ float* caretY
		)
	{
		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);

		DWRITE_HIT_TEST_METRICS metrics;

		NPE_THROW_GFX_EXCEPT(pLayout->HitTestTextPosition(
			textPos,
			isTrailingHit,
			caretX,
			caretY,
			&metrics
		), "Failed to run DWriteTextLayout::HitTestTextPosition");

		Util::Release(&pLayout);
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
	
	void TextRenderer::CreateTextLayout(_In_ const NText& text, _Out_ IDWriteTextLayout** ppLayout)
	{
		IDWriteTextFormat* pFormat;
		NPE_THROW_GFX_EXCEPT(m_pFactory->CreateTextFormat(text.fontFamily.c_str(),
			nullptr, text.fontWeight, text.fontStyle, text.fontStretch, text.fontSize,
			text.localeName.c_str(), &pFormat), "Failed to create IDWriteTextFormat"
		);

		NPE_THROW_GFX_EXCEPT(m_pFactory->CreateTextLayout(
			text.text.c_str(), (UINT32)wcslen(text.text.c_str()), pFormat,
			text.size.width, text.size.height, ppLayout), 
			"Failed to create DWriteTextLayout"
		);

		Util::Release(&pFormat);
	}
}


