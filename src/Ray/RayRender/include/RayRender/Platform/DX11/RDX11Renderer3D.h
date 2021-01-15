#pragma once

#include "../../RRBase.h"
#include "../../RRenderer3D.h"

#include "RDX11GraphicsResources.h"

#include <wrl.h>

struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct IDXGISwapChain;


namespace At0::Ray
{
	class RR_API DX11Renderer3D : public Renderer3D, DX11GraphicsResources
	{
	public:
		DX11Renderer3D(HWND hWnd);
		~DX11Renderer3D();

		virtual void Draw(Scene& scene) override;
		virtual void ClearBuffer(float red, float green, float blue) override;
		virtual void EndDraw() override;

	private:
		HWND m_hWnd;

		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSV;
	};
}