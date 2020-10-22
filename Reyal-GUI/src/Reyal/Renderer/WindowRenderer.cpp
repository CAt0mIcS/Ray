#include "rlpch.h"
#include "WindowRenderer.h"



namespace Zeal::Reyal
{
	WindowRenderer::WindowRenderer()
		: m_hWnd(0)
	{
		ZL_PROFILE_FUNCTION();

	}
	
	void WindowRenderer::Init(HWND hWnd)
	{
		ZL_PROFILE_FUNCTION();
		m_hWnd = hWnd;
		CreateGraphicsResources();
	}
	
	void WindowRenderer::CreateGraphicsResources()
	{
		ZL_PROFILE_FUNCTION();
		// TODO: Error handling

		if (!m_pFactory)
		{
#if defined(DEBUG) || defined(_DEBUG)
			//D2D1_FACTORY_OPTIONS options{ D2D1_DEBUG_LEVEL_INFORMATION };
			D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED, 
				//options, 
				m_pFactory.GetAddressOf()
			);
#else
			D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED,
				m_pFactory.GetAddressOf()
			);
#endif
		}

		m_pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(), 
			D2D1::HwndRenderTargetProperties(m_hWnd), 
			&m_pRenderTarget
		);

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1), &m_pSolidBrush);
	}
}


