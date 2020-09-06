#include "pch.h"
#include "Renderer2D.h"

#include "NPE/Util/Exceptions.h"


namespace NPE
{
	Renderer2D::Renderer2D()
		: m_hWnd(0)
	{
	}

	void Renderer2D::Init(HWND hWnd)
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

	void Renderer2D::RenderRoundedRectControl(const Control& control, float radiusX, float radiusY) const
	{
		D2D1_ROUNDED_RECT rc;

		if (radiusX == 0.0f || radiusY == 0.0f)
		{
			const auto radius = std::max(control.GetSize().width, control.GetSize().height);
			rc.radiusX = radius / 5.0f;
			rc.radiusY = radius / 5.0f;
		}
		else
		{
			rc.radiusX = radiusX;
			rc.radiusY = radiusY;
		}

		//rc.radiusX = control.GetSize().width / 5.0f; 
		//rc.radiusY = control.GetSize().height / 5.0f;

		rc.rect.left = control.GetPos().x;
		rc.rect.top = control.GetPos().y;
		rc.rect.right = rc.rect.left + control.GetSize().width;
		rc.rect.bottom = rc.rect.top + control.GetSize().height;

		m_pBrush->SetColor(control.GetColor().ToD2D1ColorF());
		m_pRenderTarget->FillRoundedRectangle(&rc, m_pBrush.Get());
	}

	void Renderer2D::RenderRoundedRectBorder(const NPoint& pos, const NSize& size, const NColor& color, float radiusX, float radiusY) const
	{
		D2D1_ROUNDED_RECT rc;

		if (radiusX == 0.0f || radiusY == 0.0f)
		{
			const auto radius = std::max(size.width, size.height);
			rc.radiusX = radius / 5.0f;
			rc.radiusY = radius / 5.0f;
		}
		else
		{
			rc.radiusX = radiusX;
			rc.radiusY = radiusY;
		}

		rc.rect.left = pos.x;
		rc.rect.top = pos.y;
		rc.rect.right = rc.rect.left + size.width;
		rc.rect.bottom = rc.rect.top + size.height;

		m_pBrush->SetColor(color.ToD2D1ColorF());
		m_pRenderTarget->DrawRoundedRectangle(&rc, m_pBrush.Get());
	}

	void Renderer2D::RenderText(const std::wstring& text, const NPoint& pos, const NColor& color, const float fontSize)
	{
		//needs to be created here to allow for custom font size
		NPE_THROW_GFX_EXCEPT(m_pWriteFactory->CreateTextFormat(L"Consolas", nullptr,
			DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &m_pTextFormat), "Failed to create IDWriteTextFormat");

		D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();

		//TODO: fix text splitting onto new line when it doesn't fit anymore
		m_pBrush->SetColor(color.ToD2D1ColorF());
		m_pRenderTarget->DrawTextW(
			text.c_str(),
			wcslen(text.c_str()),
			m_pTextFormat.Get(),
			D2D1::RectF(pos.x, pos.y, renderTargetSize.width, renderTargetSize.height),
			m_pBrush.Get()
		);

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

	bool Renderer2D::RoundedRectConrolsOverlap(const Control& cOne, const Control& cTwo, const NSize& minDst)
	{
		D2D1_ROUNDED_RECT rcOne;
		D2D1_ROUNDED_RECT rcTwo;

		//rcOne
		auto radius = std::max(cOne.GetSize().width, cOne.GetSize().height);
		rcOne.radiusX = radius / 5.0f;
		rcOne.radiusY = radius / 5.0f;

		rcOne.rect.left = cOne.GetPos().x + minDst.width;
		rcOne.rect.top = cOne.GetPos().y + minDst.height;
		rcOne.rect.right = rcOne.rect.left + cOne.GetSize().width + minDst.width;
		rcOne.rect.bottom = rcOne.rect.top + cOne.GetSize().height + minDst.height;

		ID2D1RoundedRectangleGeometry* rrgOne;
		NPE_THROW_GFX_EXCEPT(m_pFactory->CreateRoundedRectangleGeometry(rcOne, &rrgOne), "Failed to create rounded rectangle geometry (1) for overlap comparison");

		//rcTwo
		radius = std::max(cTwo.GetSize().width, cTwo.GetSize().height);
		rcTwo.radiusX = radius / 5.0f;
		rcTwo.radiusY = radius / 5.0f;

		rcTwo.rect.left = cTwo.GetPos().x + minDst.width;
		rcTwo.rect.top = cTwo.GetPos().y + minDst.height;
		rcTwo.rect.right = rcTwo.rect.left + cTwo.GetSize().width + minDst.width;
		rcTwo.rect.bottom = rcTwo.rect.top + cTwo.GetSize().height + minDst.height;

		ID2D1RoundedRectangleGeometry* rrgTwo;
		NPE_THROW_GFX_EXCEPT(m_pFactory->CreateRoundedRectangleGeometry(rcTwo, &rrgTwo), "Failed to create rounded rectangle geometry (2) for overlap comparison");

		//compare and store result in relation
		D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
		NPE_THROW_GFX_EXCEPT(rrgOne->CompareWithGeometry(rrgTwo, nullptr, &relation), "Failed to compare rounded rectangle geometries for overlap comparison");

		rrgOne->Release();
		rrgTwo->Release();

		return relation == D2D1_GEOMETRY_RELATION_CONTAINS || relation == D2D1_GEOMETRY_RELATION_OVERLAP || relation == D2D1_GEOMETRY_RELATION_IS_CONTAINED;
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

		const D2D1_COLOR_F color = D2D1::ColorF(35.0f / 255.0f, 38.0f / 255.0f, 40.0f / 255.0f);
			NPE_THROW_GFX_EXCEPT(m_pRenderTarget->CreateSolidColorBrush(color, &m_pBrush), "Failed to create D2D1SolidColorBrush");
	}
}


