#pragma once

#include "RlBase.h"

#include "RlWin.h"
#include <wrl.h>


namespace At0::Reyal
{
	class RL_API Renderer3D
	{
	public:
		Renderer3D() = default;

		void Init(HWND hWnd);
		void RenderTestTriangle();
		void EndDraw();

	private:
		HWND m_hWnd;

		Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTarget;
	};

}

