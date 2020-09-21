#include "pch.h"
#include "Renderer2D.h"

#include "Util/Exceptions.h"
#include "Util/Direct2D.h"

#include "../res/resource.h"


namespace GUI
{
	Renderer2D* Renderer2D::s_Instance = new Renderer2D();

	void Renderer2D::RenderRect(_In_ const Util::NPoint& pos, _In_ const Util::NSize& size, _In_ const Util::NColor& color)
	{
		m_pBrush->SetColor(color.ToD2D1ColorF());
		D2D1_RECT_F rc;
		rc.left = pos.x;
		rc.top = pos.y;
		rc.right = pos.x + size.width;
		rc.bottom = pos.y + size.height;

		m_pRenderTarget->FillRectangle(rc, m_pBrush.Get());
	}

	void Renderer2D::RenderRect(_In_ const D2D1_RECT_F& rect, _In_ const Util::NColor& color)
	{
		m_pBrush->SetColor(color.ToD2D1ColorF());
		m_pRenderTarget->FillRectangle(rect, m_pBrush.Get());
	}

	void Renderer2D::RenderRoundedRect(
		_In_ const Util::NPoint& pos, 
		_In_ const Util::NSize& size, 
		_In_ const Util::NColor& color, 
		_In_ const float radiusX, 
		_In_ const float radiusY
		)
	{
		m_pBrush->SetColor(color.ToD2D1ColorF());
	
		m_pRenderTarget->FillRoundedRectangle({ { pos.x, pos.y, pos.x + size.width, pos.y + size.height }, radiusX, radiusY }, m_pBrush.Get());
	}

	void Renderer2D::RenderLine(
		_In_ const Util::NPoint& startPos, 
		_In_ const Util::NPoint& endPos, 
		_In_opt_ const Util::NColor& color, 
		_In_opt_ const float radius
		)
	{
		m_pBrush->SetColor(color.ToD2D1ColorF());
		m_pRenderTarget->DrawLine(startPos.ToD2D1Point2F(), endPos.ToD2D1Point2F(), m_pBrush.Get(), radius);
	}

	Renderer2D::Renderer2D()
		: m_hWnd(0) {}

	void Renderer2D::Init(_In_ HWND hWnd)
	{
		m_hWnd = hWnd;
		CreateGraphicsResources();
	}

	void Renderer2D::BeginDraw()
	{
		m_pRenderTarget->BeginDraw();
	}

	void Renderer2D::EndDraw()
	{
		NPE_THROW_GFX_EXCEPT(m_pRenderTarget->EndDraw(), "Failed to draw object(s)");
	}

	void Renderer2D::RenderScene(_In_opt_ const Util::NColor& color)
	{
		RenderBitmapBackground();
	}

	void Renderer2D::RenderBitmapBackground()
	{
		auto rtSize = m_pRenderTarget->GetSize();

		// Create a rectangle with size of current window
		auto rectangle = D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height);
		m_pRenderTarget->DrawBitmap(m_pD2DBitmap.Get(), rectangle);
	}

	void Renderer2D::CreateGraphicsResources()
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

		D2D1_COLOR_F backgroundColor = g_DefaultWindowBackgroundColor.ToD2D1ColorF();
		NPE_THROW_GFX_EXCEPT(m_pRenderTarget->CreateSolidColorBrush(
			backgroundColor, &m_pBrush
		), "Failed to create D2D1SolidColorBrush");


		//for background bitmap
		IWICImagingFactory* pIWICFactory = nullptr;
		IWICBitmapDecoder* pDecoder = nullptr;
		IWICBitmapFrameDecode* pSource = nullptr;
		IWICStream* pStream = nullptr;
		IWICFormatConverter* pConverter = nullptr;
		IWICBitmapScaler* pScaler = nullptr;

		HRSRC imageResHandle = NULL;
		HGLOBAL imageResDataHandle = NULL;
		void* pImageFile = nullptr;
		DWORD imageFileSize = 0;

		NPE_THROW_GFX_EXCEPT(CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pIWICFactory)
		), "Failed to create WICImagingFactory");

		imageResHandle = FindResource(GetModuleHandle(L"NPEGUI.dll"), MAKEINTRESOURCE(IDI_BGND_IMAGE), L"Image");
		if (!imageResHandle)
			NPE_THROW_WND_EXCEPT(GetLastError());

		imageResDataHandle = LoadResource(GetModuleHandle(L"NPEGUI.dll"), imageResHandle);
		if (!imageResDataHandle)
			NPE_THROW_WND_EXCEPT(GetLastError());

		pImageFile = LockResource(imageResDataHandle);
		if (!pImageFile)
			NPE_THROW_WND_EXCEPT(GetLastError());

		imageFileSize = SizeofResource(GetModuleHandle(L"NPEGUI.dll"), imageResHandle);
		if (!imageFileSize)
			NPE_THROW_WND_EXCEPT(GetLastError());

		NPE_THROW_GFX_EXCEPT(pIWICFactory->CreateStream(&pStream), "Failed to create IWICStream");
		NPE_THROW_GFX_EXCEPT(pStream->InitializeFromMemory((BYTE*)pImageFile, imageFileSize), "Failed to initialize IWICStream from memory");

		NPE_THROW_GFX_EXCEPT(pIWICFactory->CreateDecoderFromStream(
			pStream, nullptr, WICDecodeMetadataCacheOnLoad, &pDecoder), "Failed to create IWICBitmapDecoder from IWICStream");

		NPE_THROW_GFX_EXCEPT(pDecoder->GetFrame(0, &pSource), "Failed to get first frame from IWICBitmapDecoder");

		NPE_THROW_GFX_EXCEPT(pIWICFactory->CreateFormatConverter(&pConverter), "Failed to create IWICFormatConverter");
		NPE_THROW_GFX_EXCEPT(pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		), "Failed to initialize IWICFormatConverter");

		NPE_THROW_GFX_EXCEPT(m_pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			&m_pD2DBitmap
		), "Failed to create bitmap from WIC bitmap");

		Util::Release(&pDecoder);
		Util::Release(&pSource);
		Util::Release(&pStream);
		Util::Release(&pConverter);
		Util::Release(&pScaler);
	}
}
