#include "RayRender/RRpch.h"

#ifdef _WIN32
#include "RDX11Renderer3D.h"

#include "../../RVertexBuffer.h"
#include "../../RIndexBuffer.h"
#include "../../RShader.h"
#include "../../RTopology.h"
#include "../../RConstantBuffers.h"

#include "../../Core/RCamera.h"

#include <RayDebug/RAssert.h>
#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>
#include <RayUtil/RComponents.h>

namespace WRL = Microsoft::WRL;


namespace At0::Ray
{
	DX11Renderer3D::DX11Renderer3D(HWND hWnd, EventDispatcher<WindowResizeEvent>& resizeDispatcher)
		: m_hWnd(hWnd), m_SyncInterval{ 0 },
		EventListener<WindowResizeEvent>(resizeDispatcher)
	{
		RAY_PROFILE_FUNCTION();

		RECT clientWindowRect;
		GetClientRect(hWnd, &clientWindowRect);

		// ----------------------------------------------------------------------------------------------------
		// Create the Swap Chain
		DXGI_SWAP_CHAIN_DESC scdesc{};
		scdesc.BufferDesc.Width = clientWindowRect.right;
		scdesc.BufferDesc.Height = clientWindowRect.bottom;
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

		RAY_DX_THROW_FAILED(GetDXGIFactory()->CreateSwapChain(GetDevice(), &scdesc, &m_pSwapChain));

		// ----------------------------------------------------------------------------------------------------
		// gain access to texture subresource in swap chain (back buffer)
		WRL::ComPtr<ID3D11Resource> pBackBuffer;
		RAY_DX_THROW_FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		RAY_DX_THROW_FAILED(GetDevice()->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pTargetView));

		// ----------------------------------------------------------------------------------------------------
		// create depth stensil state
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		WRL::ComPtr<ID3D11DepthStencilState> pDSState;
		RAY_DX_THROW_FAILED(GetDevice()->CreateDepthStencilState(&dsDesc, &pDSState));

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
		RAY_DX_THROW_FAILED(GetDevice()->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

		// ----------------------------------------------------------------------------------------------------
		// create view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;
		RAY_DX_THROW_FAILED(GetDevice()->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_pDSV));

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

	void DX11Renderer3D::ClearBuffer(float red, float green, float blue)
	{
		float color[] = { red, green, blue, 1.0f };
		GetContext()->ClearRenderTargetView(m_pTargetView.Get(), color);
		GetContext()->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void DX11Renderer3D::EndDraw()
	{
		RAY_DX_THROW_FAILED(m_pSwapChain->Present(m_SyncInterval, 0));
	}

	void DX11Renderer3D::Draw(const Camera& camera, Scene& scene)
	{
		RAY_PROFILE_FUNCTION();

		for (Ref<Drawable>& d : scene)
		{
			d->GetVertexBuffer()->Bind();
			IndexBuffer* idxBuff = d->GetIndexBuffer();
			idxBuff->Bind();

			d->GetShader()->Bind();
			d->GetTopology()->Bind();

			// RAY_TEMPORARY
			d->GetVertexConstantBuffer()->Update(
				(d->GetComponent<TransformComponent>().ToMatrix() *
					camera.GetMatrix() * camera.GetProjection()).Transpose()
			);
			d->GetVertexConstantBuffer()->Bind();

			GetContext()->DrawIndexed(idxBuff->GetIndicesCount(), 0, 0);
		}
	}

	void DX11Renderer3D::SetSyncInterval(uint32_t interval)
	{
		m_SyncInterval = interval;
	}

	void DX11Renderer3D::OnEvent(Widget& receiver, WindowResizeEvent& e)
	{
		Log::Warn("[DX11Renderer3D::OnEvent(WindowResizeEvent)] Missing implementation.");
	}
}

#endif // _WIN32



