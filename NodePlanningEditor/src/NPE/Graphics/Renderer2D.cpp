#include "pch.h"
#include "Renderer2D.h"

#include "NPE/Util/Exceptions.h"


namespace NPE
{
	Renderer2D::Renderer2D()
		: m_hWnd(0)
	{
	}

	void Renderer2D::Init(HWND hWnd, const unsigned int fontSize)
	{
		m_hWnd = hWnd;
		CreateGraphicsResources(fontSize);
	}

	void Renderer2D::BeginDraw()
	{
		m_pRenderTarget->BeginDraw();
	}

	void Renderer2D::EndDraw()
	{
		NPE_THROW_GFX_EXCEPT(m_pRenderTarget->EndDraw(), "Failed to draw object(s)");
	}

	void Renderer2D::RenderRoundedRectControl(const Control& control) const
	{
		D2D1_ROUNDED_RECT rc;

		const auto radius = std::max(control.GetSize().width, control.GetSize().height);
		rc.radiusX = radius / 5.0f;
		rc.radiusY = radius / 5.0f;

		//rc.radiusX = control.GetSize().width / 5.0f; 
		//rc.radiusY = control.GetSize().height / 5.0f;

		rc.rect.left = control.GetPos().x;
		rc.rect.top = control.GetPos().y;
		rc.rect.right = rc.rect.left + control.GetSize().width;
		rc.rect.bottom = rc.rect.top + control.GetSize().height;

		m_pBrush->SetColor(control.GetColor().ToD2D1ColorF());
		m_pRenderTarget->FillRoundedRectangle(&rc, m_pBrush.Get());
	}

	void Renderer2D::RenderText(const std::wstring text, const NPoint& pos, const NSize& size)
	{
		//D2D1_RECT_F layoutRect;
		//layoutRect.left = pos.x;
		//layoutRect.top = pos.y;
		//layoutRect.right = pos.x + size.width;
		//layoutRect.bottom = pos.y + size.height;

		D2D1_RECT_F layoutRect;
		layoutRect.left = 10;
		layoutRect.top = 10;
		layoutRect.right = 300;
		layoutRect.bottom = 300;

		//TEMPORARY: 
		m_pBrush->SetColor({ 255, 255, 255 });
		//m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat.Get(),
		//	layoutRect, m_pBrush.Get());

	}

	void Renderer2D::RenderLine(const NPoint& startPos, const NPoint& endPos, const NColor& color, const unsigned int width)
	{
		m_pBrush->SetColor(color.ToD2D1ColorF());
		m_pRenderTarget->DrawLine(startPos.ToD2D1Point2F(), endPos.ToD2D1Point2F(), m_pBrush.Get(), width);
	}

	void Renderer2D::RenderScene(const NColor& color)
	{
		m_pRenderTarget->Clear(color.ToD2D1ColorF());
	}

	void Renderer2D::CreateGraphicsResources(const unsigned int fontSize)
	{
		NPE_THROW_GFX_EXCEPT(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,
			__uuidof(m_pFactory), &m_pFactory), "Failed to create D2D1Factory");

		RECT rc;
		GetClientRect(m_hWnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		NPE_THROW_GFX_EXCEPT(m_pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, size), &m_pRenderTarget), "Failed to create D2D1HwndRenderTarget");

		NPE_THROW_GFX_EXCEPT(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pWriteFactory), &m_pWriteFactory), "Failed to create DWriteFactory");

		NPE_THROW_GFX_EXCEPT(m_pWriteFactory->CreateTextFormat(L"Consolas", nullptr,
			DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &m_pTextFormat), "Failed to create IDWriteTextFormat");

		const D2D1_COLOR_F color = D2D1::ColorF(35.0f / 255.0f, 38.0f / 255.0f, 40.0f / 255.0f);
			NPE_THROW_GFX_EXCEPT(m_pRenderTarget->CreateSolidColorBrush(color, &m_pBrush), "Failed to create D2D1SolidColorBrush");
	}
}


