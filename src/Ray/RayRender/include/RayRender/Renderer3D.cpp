#include "RayRender/rlrpch.h"
#include "Renderer3D.h"

#include <RayUtil/RException.h>
#include <RayDebug/RAssert.h>
#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>


namespace WRL = Microsoft::WRL;

namespace At0::Ray
{
	Renderer3D::Renderer3D(EventDispatcher<WindowResizeEvent>& dispatcher)
		: m_Projection(DirectX::XMMatrixIdentity()), m_pSwapChain(nullptr), m_pTarget(nullptr), m_hWnd(0),
		EventListener<WindowResizeEvent>(dispatcher)
	{
		RAY_PROFILE_FUNCTION();
	}

	void Renderer3D::Init(HWND hWnd)
	{
		RAY_PROFILE_FUNCTION();

		auto swapChain = GetSwapChain();
		auto context = GetContext();
		RAY_MEXPECTS(!m_pSwapChain, "D3D11RendererAPI was already initialized.");
		m_hWnd = hWnd;

		RECT clientWindowRect;
		RAY_WND_THROW_LAST_FAILED(GetClientRect(m_hWnd, &clientWindowRect));


		// ----------------------------------------------------------------------------------------------------
		// Swap Chain
		DXGI_SWAP_CHAIN_DESC sd{};

		// Let DXGI figure it out with hWnd
		sd.BufferDesc.Width = clientWindowRect.right;
		sd.BufferDesc.Height = clientWindowRect.bottom;

		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate = { 0, 0 };
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = m_hWnd;

		sd.SampleDesc = { 1, 0 };
		sd.BufferCount = 1;
		sd.Windowed = TRUE;

		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		RAY_GFX_THROW_FAILED(s_pIDXGIFactory->CreateSwapChain(s_pDevice, &sd, &swapChain));


		// ----------------------------------------------------------------------------------------------------
		// gain access to texture subresource in swap chain (back buffer)
		WRL::ComPtr<ID3D11Resource> pBackBuffer;
		RAY_GFX_THROW_FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		RAY_GFX_THROW_FAILED(s_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &GetTarget()));

		//// ----------------------------------------------------------------------------------------------------
		//// create depth stensil state
		//D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		//dsDesc.DepthEnable = TRUE;
		//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		//WRL::ComPtr<ID3D11DepthStencilState> pDSState;
		//RAY_GFX_THROW_FAILED(s_pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

		//// ----------------------------------------------------------------------------------------------------
		//// bind depth state
		//context->OMSetDepthStencilState(pDSState.Get(), 1u);

		//// ----------------------------------------------------------------------------------------------------
		//// create depth stensil texture
		//WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
		//D3D11_TEXTURE2D_DESC descDepth = {};
		//descDepth.Width = clientWindowRect.right - clientWindowRect.left;
		//descDepth.Height = clientWindowRect.bottom - clientWindowRect.top;
		//descDepth.MipLevels = 1u;
		//descDepth.ArraySize = 1u;
		//descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		//descDepth.SampleDesc.Count = 1u;
		//descDepth.SampleDesc.Quality = 0u;
		//descDepth.Usage = D3D11_USAGE_DEFAULT;
		//descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		//RAY_GFX_THROW_FAILED(s_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

		//// ----------------------------------------------------------------------------------------------------
		//// create view of depth stensil texture
		//D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		//descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		//descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		//descDSV.Texture2D.MipSlice = 0u;
		//RAY_GFX_THROW_FAILED(s_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_pDSV));

		// ----------------------------------------------------------------------------------------------------
		// bind depth stensil view to OM
		//context->OMSetRenderTargets(1u, GetTarget().GetAddressOf(), m_pDSV.Get());
		//context->OMSetRenderTargets(1u, GetTarget().GetAddressOf(), nullptr);

		// ----------------------------------------------------------------------------------------------------
		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = clientWindowRect.right;
		vp.Height = clientWindowRect.bottom;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		context->RSSetViewports(1u, &vp);
	}

	void Renderer3D::DrawIndexed(uint32_t indicesCount)
	{
		GetContext()->DrawIndexed(indicesCount, 0, 0);
	}

	void Renderer3D::ClearBuffer(float red, float green, float blue)
	{
		const float color[] = { red,green,blue,1.0f };

		auto target = GetTarget();
		auto context = GetContext();
		context->ClearRenderTargetView(target.Get(), color);
		context->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void Renderer3D::EndDraw()
	{
		// VSYNC on
		//RAY_GFX_THROW_FAILED(m_pSwapChain->Present(1, 0));

		// ------------------------------------------------------------------------------------------------------------------
		// HAPPENS HERE WHEN RESIZING SOMETIMES:
		// D3D11: Removing Device.
		// D3D11 ERROR: ID3D11Device::RemoveDevice: 
		// Device removal has been triggered for the following reason 
		// (DXGI_ERROR_DEVICE_HUNG: The Device took an unreasonable amount of time to execute its commands,
		// or the hardware crashed/hung. As a result, the TDR 
		// (Timeout Detection and Recovery) mechanism has been triggered. The current Device Context was executing 
		// commands when the hang occurred. The application may want to respawn and fallback to less aggressive 
		// use of the display hardware). [ EXECUTION ERROR #378: DEVICE_REMOVAL_PROCESS_AT_FAULT]
		// ------------------------------------------------------------------------------------------------------------------

		// VSYNC off
		RAY_GFX_THROW_FAILED(GetSwapChain()->Present(0, 0));
	}

	Renderer3D::~Renderer3D()
	{
		RAY_PROFILE_FUNCTION();
		Log::Debug("[Renderer3D] Destroyed");
	}

	void Renderer3D::OnEvent(Widget& receiver, WindowResizeEvent& e)
	{
		auto swapChain = GetSwapChain();
		RAY_MEXPECTS(swapChain.Get() != nullptr, "[Renderer3D::OnEvent] SwapChain invalid. Renderer not initialized");
		auto context = GetContext();
		{
			RECT clientWindowRect;
			RAY_WND_THROW_LAST_FAILED(GetClientRect(m_hWnd, &clientWindowRect));

			context->OMSetRenderTargets(0, nullptr, nullptr);

			auto target = GetTarget();
			target.ReleaseAndGetAddressOf();
			RAY_GFX_THROW_FAILED(swapChain->ResizeBuffers(0, clientWindowRect.right, clientWindowRect.bottom, DXGI_FORMAT_UNKNOWN, 0));

			WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
			RAY_GFX_THROW_FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
			RAY_GFX_THROW_FAILED(s_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &target));

			context->OMSetRenderTargets(1u, target.GetAddressOf(), nullptr);

			//// ----------------------------------------------------------------------------------------------------
			//// create depth stensil texture
			//WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
			//D3D11_TEXTURE2D_DESC descDepth = {};
			//descDepth.Width = clientWindowRect.right - clientWindowRect.left;
			//descDepth.Height = clientWindowRect.bottom - clientWindowRect.top;
			//descDepth.MipLevels = 1u;
			//descDepth.ArraySize = 1u;
			//descDepth.Format = DXGI_FORMAT_D32_FLOAT;
			//descDepth.SampleDesc.Count = 1u;
			//descDepth.SampleDesc.Quality = 0u;
			//descDepth.Usage = D3D11_USAGE_DEFAULT;
			//descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			//RAY_GFX_THROW_FAILED(s_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

			//	// ----------------------------------------------------------------------------------------------------
			//	// create view of depth stensil texture
			//	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
			//	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
			//	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			//	descDSV.Texture2D.MipSlice = 0u;
			//	RAY_GFX_THROW_FAILED(s_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_pDSV));

			//	// ----------------------------------------------------------------------------------------------------
			//	// bind depth stensil view to OM
			//	context->OMSetRenderTargets(1u, target.GetAddressOf(), m_pDSV.Get());
		}

		const Size2& size = e.GetSize();
		D3D11_VIEWPORT vp;
		vp.Width = size.x;
		vp.Height = size.y;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		context->RSSetViewports(1, &vp);

		// TEMPORARY! Need customization for near and far
		SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 500.0f));
	}
}