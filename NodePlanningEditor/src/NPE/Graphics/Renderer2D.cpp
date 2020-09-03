#include "pch.h"
#include "Renderer2D.h"


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

	void Renderer2D::RenderControl(Control& control)
	{

	}

	void Renderer2D::RenderText(const std::wstring text, const NPoint& pos, const NSize& size)
	{

	}

	void Renderer2D::RenderScene(const NColor& color)
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->Clear(color.ToD2D1ColorF());

		if (FAILED(m_pRenderTarget->EndDraw())) 
		{ 
			throw std::exception();
		}
	}

	void Renderer2D::CreateGraphicsResources(const unsigned int fontSize)
	{
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(m_pFactory), &m_pFactory)))
		{
			throw std::exception();
		}

		RECT rc;
		GetClientRect(m_hWnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		if (FAILED(m_pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, size), &m_pRenderTarget)))
		{
			throw std::exception();
		}

		if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pWriteFactory), &m_pWriteFactory)))
		{
			throw std::exception();
		}

		if (FAILED(m_pWriteFactory->CreateTextFormat(L"Consolas", nullptr,
			DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &m_pTextFormat)))
		{
			throw std::exception();
		}

		const D2D1_COLOR_F color = D2D1::ColorF(35.0f / 255.0f, 38.0f / 255.0f, 40.0f / 255.0f);
		if (FAILED(m_pRenderTarget->CreateSolidColorBrush(color, &m_pBrush)))
		{
			throw std::exception();
		}
	}
}


