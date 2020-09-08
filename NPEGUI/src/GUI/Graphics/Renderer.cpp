#include "pch.h"
#include "Renderer.h"

#include "Util/Exceptions.h"


namespace GUI
{
	Renderer* Renderer::s_Instance = new Renderer();

	void Renderer::RenderRect(const Util::NPoint& pos, const Util::NSize& size, const Util::NColor& color)
	{
		m_pBrush->SetColor(color.ToD2D1ColorF());
		D2D1_RECT_F rc;
		rc.left = pos.x;
		rc.top = pos.y;
		rc.right = pos.x + size.width;
		rc.bottom = pos.y + size.height;

		m_pRenderTarget->FillRectangle(rc, m_pBrush.Get());
	}

	void Renderer::RenderRoundedRect(const Util::NPoint& pos, const Util::NSize& size, const Util::NColor& color, const float radiusX, const float radiusY)
	{
		m_pBrush->SetColor(color.ToD2D1ColorF());
	
		m_pRenderTarget->FillRoundedRectangle({ { pos.x, pos.y, pos.x + size.width, pos.y + size.height }, radiusX, radiusY }, m_pBrush.Get());
	}

	Renderer::Renderer()
		: m_hWnd(0) {}

	void Renderer::Init(HWND hWnd)
	{
		m_hWnd = hWnd;
		CreateGraphicsResources();
	}

	void Renderer::BeginDraw()
	{
		m_pRenderTarget->BeginDraw();
	}

	void Renderer::EndDraw()
	{
		NPE_THROW_GFX_EXCEPT(m_pRenderTarget->EndDraw(), "Failed to draw object(s)");
	}

	void Renderer::RenderScene(const Util::NColor& color)
	{
		RenderBitmapBackground();
	}

	void Renderer::RenderBitmapBackground()
	{
		auto rtSize = m_pRenderTarget->GetSize();

		// Create a rectangle with size of current window
		auto rectangle = D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height);
		m_pRenderTarget->DrawBitmap(m_pD2DBitmap.Get(), rectangle);
	}

	void Renderer::CreateGraphicsResources()
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

		D2D1_COLOR_F col;
		col.r = g_DefaultWindowBackgroundColor.r;
		col.g = g_DefaultWindowBackgroundColor.g;
		col.b = g_DefaultWindowBackgroundColor.b;
		col.a = 1.0f;

		NPE_THROW_GFX_EXCEPT(m_pRenderTarget->CreateSolidColorBrush(
			col, &m_pBrush
		), "Failed to create D2D1SolidColorBrush");


		//for background bitmap
		IWICImagingFactory* pIWICFactory;
		IWICFormatConverter* pConvertedSourceBitmap;
		IWICBitmapDecoder* pDecoder;
		IWICBitmapFrameDecode* pFrame;

		NPE_THROW_GFX_EXCEPT(CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pIWICFactory)
		), "Failed to create WICImagingFactory");

		NPE_THROW_GFX_EXCEPT(pIWICFactory->CreateDecoderFromFilename(
			L"BackgroundImage.bmp",
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&pDecoder
		), "");


		NPE_THROW_GFX_EXCEPT(pDecoder->GetFrame(0, &pFrame), "Failed to get decoded bitmap frame");
		pDecoder->Release();

		NPE_THROW_GFX_EXCEPT(pIWICFactory->CreateFormatConverter(&pConvertedSourceBitmap), "Failed to format frame to 32bppPBGRA");
		pIWICFactory->Release();

		NPE_THROW_GFX_EXCEPT(pConvertedSourceBitmap->Initialize(
			pFrame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.f,
			WICBitmapPaletteTypeCustom
		), "Failed to initialize converted source bitmap");
		pFrame->Release();

		NPE_THROW_GFX_EXCEPT(m_pRenderTarget->CreateBitmapFromWicBitmap(
			pConvertedSourceBitmap, nullptr, &m_pD2DBitmap
		), "Failed to create bitmap from wic bitmap");
		pConvertedSourceBitmap->Release();
	}
}
