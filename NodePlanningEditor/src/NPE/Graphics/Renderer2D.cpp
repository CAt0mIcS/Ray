#include "pch.h"
#include "Renderer2D.h"

#pragma warning(suppress: 4996)


namespace NPE
{
	Renderer2D::Renderer2D(HWND hWnd)
		: m_hWnd(hWnd)
	{
		InitDPIScale();
		InitGraphicsResources();
	}

	void Renderer2D::Init(HWND hWnd)
	{
		m_hWnd = hWnd;
		InitDPIScale();
		InitGraphicsResources();
	}

	void Renderer2D::Draw()
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->Clear(D2D1::ColorF(35.0f / 255, 38.0f / 255, 40.0f / 255));
		if (FAILED(m_pRenderTarget->EndDraw())) throw std::exception("Failed to draw node");
	}

	void Renderer2D::DrawNode(const NodeRect& node)
	{
		m_pRenderTarget->BeginDraw();
		
		m_pBrush->SetColor(D2D1::ColorF(0.058f, 0.066f, 0.074f));
		m_pRenderTarget->FillRoundedRectangle(node.GetRect(), m_pBrush.Get());

		m_pBrush->SetColor(D2D1::ColorF(0, 0, 0));
		m_pRenderTarget->DrawRoundedRectangle(node.GetRect(), m_pBrush.Get(), 1.0f);
		if (FAILED(m_pRenderTarget->EndDraw())) throw std::exception("Failed to draw node");
	}

	void Renderer2D::InitGraphicsResources()
	{
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pFactory.GetAddressOf()))) throw std::exception("Failed to create D2D1Factory");

		if (!m_pRenderTarget)
		{
			HRESULT hr;
			RECT rc;
			GetClientRect(m_hWnd, &rc);

			hr = m_pFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(rc.right, rc.bottom)),
				&m_pRenderTarget);

			if (FAILED(hr)) throw std::exception("Failed to create HWND Render Target");

			const D2D1_COLOR_F color = D2D1::ColorF(0.058f, 0.066f, 0.074f);
			hr = m_pRenderTarget->CreateSolidColorBrush(color, &m_pBrush);

			if (FAILED(hr)) throw std::exception("Failed to create Solid Color Brush");
		}
	}

	void Renderer2D::InitDPIScale()
	{
		UINT dpi = GetDpiForWindow(m_hWnd);
		m_DPIScale = dpi / 96.0f;
	}
}
