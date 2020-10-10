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

	DWrite::TextMetrics TextRenderer::GetTextMetrics(_In_ const NText& text)
	{
		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);

		DWrite::TextMetrics metrics;

		NPE_THROW_GFX_EXCEPT(pLayout->GetMetrics(&metrics), "Failed to get text metrics");
		Util::Release(&pLayout);

		return metrics;
	}

	std::vector<DWrite::LineMetrics> TextRenderer::GetLineMetrics(_In_ const NText& text)
	{
		std::vector<DWrite::LineMetrics> lineMetrics;

		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);

		DWrite::TextMetrics metrics = GetTextMetrics(text);
		lineMetrics.resize(metrics.lineCount);

		NPE_THROW_GFX_EXCEPT(pLayout->GetLineMetrics(&lineMetrics.front(), metrics.lineCount, &metrics.lineCount), "Failed to get line metrics");
		Util::Release(&pLayout);
		return lineMetrics;
	}

	std::vector<DWrite::ClusterMetrics> TextRenderer::GetClusterMetrics(
		_In_ const NText& text, 
		_Out_ unsigned int* clusterCount
		)
	{
		std::vector<DWrite::ClusterMetrics> clusterMetrics;

		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);

		pLayout->GetClusterMetrics(nullptr, 0, clusterCount);
		if (clusterCount == 0)
			return {};

		clusterMetrics.resize(*clusterCount);
		NPE_THROW_GFX_EXCEPT(pLayout->GetClusterMetrics(&clusterMetrics.front(), *clusterCount, clusterCount), "Failed to get cluster metrics");

		Util::Release(&pLayout);
		return clusterMetrics;
	}

	DWrite::OverhangMetrics TextRenderer::GetOverhangMetrics(_In_ const NText& text)
	{
		DWrite::OverhangMetrics overhangMetrics;
		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);
		
		NPE_THROW_GFX_EXCEPT(pLayout->GetOverhangMetrics(&overhangMetrics), "Failed to get overhang metrics");
		Util::Release(&pLayout);

		return overhangMetrics;
	}

	void TextRenderer::RenderText(_In_ const NText& text)
	{
		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);

		Renderer::Get().m_pBrush->SetColor(text.color.ToD2D1ColorF());
		Renderer::Get().m_pRenderTarget->DrawTextLayout(text.pos.ToD2D1Point2F(), pLayout, Renderer::Get().m_pBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
		Util::Release(&pLayout);
	}

	DWrite::HitTestMetrics TextRenderer::HitTestPoint(
		_In_ const NText& text, 
		_Out_ BOOL* isTrailingHit, 
		_Out_ BOOL* isInside
		)
	{
		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);

		DWrite::HitTestMetrics metrics;

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

	DWrite::HitTestMetrics TextRenderer::HitTestTextPosition(
		_In_ const NText& text, 
		_In_ unsigned int textPos, 
		_In_ BOOL isTrailingHit, 
		_Out_ float* caretX, 
		_Out_ float* caretY
		)
	{
		IDWriteTextLayout* pLayout;
		CreateTextLayout(text, &pLayout);

		DWrite::HitTestMetrics metrics;

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
	
	DWrite::Matrix TextRenderer::GetViewMatrix(_In_ const Util::NPoint& origin, _In_ const Util::NSize& scale)
	{
		RECT rect;
		GetClientRect(Renderer::Get().GetNativeWindow(), &rect);

		DWrite::Matrix translationMatrix = {
			1, 0,
			0, 1,
			-origin.x, -origin.y
		};

		double cosValue = 1.0;
		double sinValue = 0.0;

		DWrite::Matrix rotationMatrix = {
			float(cosValue * scale.width), float(sinValue * scale.width),
			float(-sinValue * scale.height), float(cosValue * scale.height),
			0, 0
		};

		float centeringFactor = 0.5f;
		DWRITE_MATRIX centerMatrix = {
			1, 0,
			0, 1,
			(float)floor((float)(rect.right * centeringFactor)), (float)floor((float)(rect.bottom * centeringFactor))
		};

		D2D1::Matrix3x2F resultA, resultB;

		resultB.SetProduct(*(D2D1::Matrix3x2F*)&translationMatrix, *(D2D1::Matrix3x2F*)&rotationMatrix);
		resultA.SetProduct(resultB, *(D2D1::Matrix3x2F*)&centerMatrix);

		// For better pixel alignment (less blurry text)
		resultA._31 = (float)floor(resultA._31);
		resultA._32 = (float)floor(resultA._32);

		return *(DWrite::Matrix*)&resultA;
	}
	
	DWrite::Matrix TextRenderer::GetTransform()
	{
		D2D1::Matrix3x2F transform;
		Renderer::Get().m_pRenderTarget->GetTransform(&transform);
		return *(DWrite::Matrix*)&transform;
	}

	DWrite::Matrix TextRenderer::SetTransform(_In_ const DWrite::Matrix& transform)
	{
		Renderer::Get().m_pRenderTarget->SetTransform((D2D1::Matrix3x2F*)&transform);
	}
}


