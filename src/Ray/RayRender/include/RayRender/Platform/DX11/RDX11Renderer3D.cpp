#include "RayRender/RRpch.h"
#include "RDX11Renderer3D.h"

#include <RayDebug/RAssert.h>
#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>

namespace WRL = Microsoft::WRL;


namespace At0::Ray
{
	DX11Renderer3D::DX11Renderer3D(HWND hWnd)
		: m_hWnd(hWnd)
	{
		RAY_PROFILE_FUNCTION();

		RECT clientWindowRect;
		GetClientRect(hWnd, &clientWindowRect);

		// ----------------------------------------------------------------------------------------------------
		// Create the Swap Chain
		DXGI_SWAP_CHAIN_DESC scdesc{};
		scdesc.BufferDesc.Width = clientWindowRect.right - clientWindowRect.left;
		scdesc.BufferDesc.Height = clientWindowRect.bottom - clientWindowRect.top;
		scdesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scdesc.BufferDesc.RefreshRate = { 0, 0 };
		scdesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scdesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		scdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scdesc.OutputWindow = m_hWnd;

		scdesc.SampleDesc = { 1, 0 };
		scdesc.BufferCount = 1;
		scdesc.Windowed = TRUE;

		scdesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scdesc.Flags = 0;

		RAY_GFX_THROW_FAILED(GetDXGIFactory()->CreateSwapChain(GetDevice(), &scdesc, &m_pSwapChain));

		// ----------------------------------------------------------------------------------------------------
		// gain access to texture subresource in swap chain (back buffer)
		WRL::ComPtr<ID3D11Resource> pBackBuffer;
		RAY_GFX_THROW_FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		RAY_GFX_THROW_FAILED(GetDevice()->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pTargetView));

		// ----------------------------------------------------------------------------------------------------
		// create depth stensil state
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		WRL::ComPtr<ID3D11DepthStencilState> pDSState;
		RAY_GFX_THROW_FAILED(GetDevice()->CreateDepthStencilState(&dsDesc, &pDSState));

		// ----------------------------------------------------------------------------------------------------
		// bind depth state
		GetContext()->OMSetDepthStencilState(pDSState.Get(), 1u);

		// ----------------------------------------------------------------------------------------------------
		// create depth stensil texture
		WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = clientWindowRect.right - clientWindowRect.left;
		descDepth.Height = clientWindowRect.bottom - clientWindowRect.top;
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		RAY_GFX_THROW_FAILED(GetDevice()->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

		// ----------------------------------------------------------------------------------------------------
		// create view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;
		RAY_GFX_THROW_FAILED(GetDevice()->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_pDSV));

		// ----------------------------------------------------------------------------------------------------
		// bind depth stensil view to OM
		GetContext()->OMSetRenderTargets(1u, m_pTargetView.GetAddressOf(), m_pDSV.Get());

		// ----------------------------------------------------------------------------------------------------
		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = clientWindowRect.right;
		vp.Height = clientWindowRect.bottom;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		GetContext()->RSSetViewports(1u, &vp);
	}

	DX11Renderer3D::~DX11Renderer3D()
	{
		RAY_PROFILE_FUNCTION();
	}
}


