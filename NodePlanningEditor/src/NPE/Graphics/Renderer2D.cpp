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

	void Renderer2D::BeginDraw()
	{
		m_pRenderTarget->BeginDraw();
	}

	void Renderer2D::EndDraw()
	{
		if (FAILED(m_pRenderTarget->EndDraw()))
		{
			throw std::exception();
		}
	}

	void Renderer2D::RenderRoundedRectControl(Control& control)
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

	}

	void Renderer2D::RenderScene(const NColor& color)
	{
		m_pRenderTarget->Clear(color.ToD2D1ColorF());
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


