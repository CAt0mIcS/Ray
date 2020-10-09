#include "pch.h"
#include "Renderer.h"

#include "Util/Exceptions.h"
#include "Util/Direct2D.h"

#include "../res/resource.h"


namespace GUI
{
	Renderer* Renderer::s_Instance = new Renderer();

	void Renderer::RenderRect(_In_ const Util::NPoint& pos, _In_ const Util::NSize& size, _In_ const Util::NColor& color)
	{
		m_pBrush->SetColor(color.ToD2D1ColorF());
		Direct2D::RectF rc;
		rc.left = pos.x;
		rc.top = pos.y;
		rc.right = pos.x + size.width;
		rc.bottom = pos.y + size.height;

		m_pRenderTarget->FillRectangle(rc, m_pBrush.Get());
	}

	void Renderer::RenderRect(_In_ const Direct2D::RectF& rect, _In_ const Util::NColor& color)
	{
		m_pBrush->SetColor(color.ToD2D1ColorF());
		m_pRenderTarget->FillRectangle(rect, m_pBrush.Get());
	}

	void Renderer::RenderRoundedRect(
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

	void Renderer::RenderLine(
		_In_ const Util::NPoint& startPos, 
		_In_ const Util::NPoint& endPos, 
		_In_opt_ const Util::NColor& color, 
		_In_opt_ const float radius
		)
	{
		m_pBrush->SetColor(color.ToD2D1ColorF());
		m_pRenderTarget->DrawLine(startPos.ToD2D1Point2F(), endPos.ToD2D1Point2F(), m_pBrush.Get(), radius);
	}

	Renderer::Renderer()
		: m_hWnd(0), m_pD2DBitmap(nullptr) {}

	void Renderer::Init(_In_ HWND hWnd)
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

	void Renderer::RenderScene(_In_opt_ const Util::NColor& color)
	{
		RenderBitmapBackground();
	}

	void Renderer::RenderBitmapBackground()
	{
		auto rtSize = m_pRenderTarget->GetSize();

		// Create a rectangle with size of current window
		auto rectangle = D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height);
		m_pRenderTarget->DrawBitmap(m_pD2DBitmap, rectangle);
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

		D2D1_COLOR_F backgroundColor = Constants::g_DefaultWindowBackgroundColor.ToD2D1ColorF();
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

		NPE_THROW_WND_EXCEPT(CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pIWICFactory))
		);

		imageResHandle = FindResource(GetModuleHandle(L"NPEGUI.dll"), MAKEINTRESOURCE(IDI_BGND_IMAGE), L"Image");
		if (!imageResHandle)
			NPE_THROW_LAST_WND_EXCEPT();

		imageResDataHandle = LoadResource(GetModuleHandle(L"NPEGUI.dll"), imageResHandle);
		if (!imageResDataHandle)
			NPE_THROW_LAST_WND_EXCEPT();

		pImageFile = LockResource(imageResDataHandle);
		if (!pImageFile)
			NPE_THROW_LAST_WND_EXCEPT();

		imageFileSize = SizeofResource(GetModuleHandle(L"NPEGUI.dll"), imageResHandle);
		if (!imageFileSize)
			NPE_THROW_LAST_WND_EXCEPT();

		NPE_THROW_WND_EXCEPT(pIWICFactory->CreateStream(&pStream));
		NPE_THROW_WND_EXCEPT(pStream->InitializeFromMemory((BYTE*)pImageFile, imageFileSize));

		NPE_THROW_WND_EXCEPT(pIWICFactory->CreateDecoderFromStream(
			pStream, nullptr, WICDecodeMetadataCacheOnLoad, &pDecoder));

		NPE_THROW_WND_EXCEPT(pDecoder->GetFrame(0, &pSource));

		NPE_THROW_WND_EXCEPT(pIWICFactory->CreateFormatConverter(&pConverter));
		NPE_THROW_WND_EXCEPT(pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut)
		);

		NPE_THROW_WND_EXCEPT(m_pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			&m_pD2DBitmap)
		);

		Util::Release(&pDecoder);
		Util::Release(&pSource);
		Util::Release(&pStream);
		Util::Release(&pConverter);
		Util::Release(&pScaler);
	}
}
