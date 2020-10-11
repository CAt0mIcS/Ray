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
		m_pBrush->SetColor(color);
		Direct2D::RectF rc;
		rc.left = pos.x;
		rc.top = pos.y;
		rc.right = pos.x + size.width;
		rc.bottom = pos.y + size.height;

		m_pRenderTarget->FillRectangle(rc, m_pBrush.Get());
	}

	void Renderer::RenderRect(_In_ const Direct2D::RectF& rect, _In_ const Util::NColor& color)
	{
		m_pBrush->SetColor(color);
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
		m_pBrush->SetColor(color);
		
		m_pRenderTarget->FillRoundedRectangle({ { pos.x, pos.y, pos.x + size.width, pos.y + size.height }, radiusX, radiusY }, m_pBrush.Get());
	}

	void Renderer::RenderLine(
		_In_ const Util::NPoint& startPos, 
		_In_ const Util::NPoint& endPos, 
		_In_opt_ const Util::NColor& color, 
		_In_opt_ const float radius
		)
	{
		m_pBrush->SetColor(color);
		m_pRenderTarget->DrawLine(startPos, endPos, m_pBrush.Get(), radius);
	}

	Renderer::Renderer()
		: m_hWnd(0), m_pD2DBitmap(nullptr), m_Scale{ 1.0f, 1.0f }, m_Origin{ 0.0f, 0.0f } {}

	void Renderer::Init(_In_ HWND hWnd)
	{
		m_hWnd = hWnd;

		RECT rc;
		GetClientRect(m_hWnd, &rc);

		m_Origin.x = float(rc.right - rc.left) / 2.0f;
		m_Origin.y = float(rc.bottom - rc.top) / 2.0f;

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

	void Renderer::RenderScene()
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

		NPE_THROW_GFX_EXCEPT(m_pRenderTarget->CreateSolidColorBrush(
			Constants::g_DefaultWindowBackgroundColor, &m_pBrush
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

	D2D1::Matrix3x2F Renderer::GetViewMatrix()
	{
		RECT rect;
		GetClientRect(m_hWnd, &rect);

		DWrite::Matrix translationMatrix = {
			1, 0,
			0, 1,
			-m_Origin.x, -m_Origin.y
		};

		double cosValue = 1.0;
		double sinValue = 0.0;

		DWrite::Matrix rotationMatrix = {
			float(cosValue * m_Scale.width), float(sinValue * m_Scale.width),
			float(-sinValue * m_Scale.height), float(cosValue * m_Scale.height),
			0, 0
		};

		float centeringFactor = 0.5f;
		DWrite::Matrix centerMatrix = {
			1, 0,
			0, 1,
			(float)floor((float)(rect.right * centeringFactor)), (float)floor((float)(rect.bottom * centeringFactor))
		};

		D2D1::Matrix3x2F resultA, resultB;

		resultB.SetProduct(*(D2D1::Matrix3x2F*)&translationMatrix, *(D2D1::Matrix3x2F*)&rotationMatrix);
		resultA.SetProduct(resultB, *(D2D1::Matrix3x2F*)&centerMatrix);

		resultA._31 = (float)floor(resultA._31);
		resultA._32 = (float)floor(resultA._32);

		return resultA;
	}

	D2D1::Matrix3x2F Renderer::GetInverseViewMatrix()
	{
		return ComputeInverseMatrix(GetViewMatrix());
	}

	D2D1::Matrix3x2F Renderer::ComputeInverseMatrix(_In_ const D2D1::Matrix3x2F& matrix)
	{
		auto GetDeterminant = [](const D2D1::Matrix3x2F& matrix)
		{
			return matrix.m11 * matrix.m22 - matrix.m12 * matrix.m21;
		};

		D2D1::Matrix3x2F result{};
		
		float invdet = 1.0f / GetDeterminant(matrix);
		result.m11 = matrix.m22 * invdet;
		result.m12 = -matrix.m12 * invdet;
		result.m21 = -matrix.m21 * invdet;
		result.m22 = matrix.m11 * invdet;
		result.dx = (matrix.m21 * matrix.dy - matrix.dx * matrix.m22) * invdet;
		result.dy = (matrix.dx * matrix.m12 - matrix.m11 * matrix.dy) * invdet;

		return result;
	}

	D2D1::Matrix3x2F Renderer::GetTransform()
	{
		D2D1::Matrix3x2F transform;
		m_pRenderTarget->GetTransform(&transform);
		return transform;
	}

	void Renderer::SetTransform(_In_ const D2D1::Matrix3x2F& transform)
	{
		m_pRenderTarget->SetTransform(transform);
	}
}
