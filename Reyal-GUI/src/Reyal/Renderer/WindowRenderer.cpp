#include "rlpch.h"
#include "WindowRenderer.h"



namespace Zeal::Reyal
{
	WindowRenderer::WindowRenderer()
		: m_hWnd(0)
	{

	}
	
	void WindowRenderer::Init(HWND hWnd)
	{
		m_hWnd = hWnd;
		CreateGraphicsResources();
	}
	
	void WindowRenderer::CreateGraphicsResources()
	{
		// TODO: Error handling

		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pFactory.GetAddressOf());

		m_pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(), 
			D2D1::HwndRenderTargetProperties(m_hWnd), 
			&m_pRenderTarget
		);

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1), &m_pSolidBrush);
	}
}


