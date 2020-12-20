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

        RAY_GFX_THROW_FAILED(s_pIDXGIFactory->CreateSwapChain(s_pDevice, &sd, &m_pSwapChain));


        // ----------------------------------------------------------------------------------------------------
        // gain access to texture subresource in swap chain (back buffer)
        WRL::ComPtr<ID3D11Resource> pBackBuffer;
        RAY_GFX_THROW_FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
        RAY_GFX_THROW_FAILED(s_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pTarget));

        // ----------------------------------------------------------------------------------------------------
        // create depth stensil state
        D3D11_DEPTH_STENCIL_DESC dsDesc = {};
        dsDesc.DepthEnable = TRUE;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
        WRL::ComPtr<ID3D11DepthStencilState> pDSState;
        RAY_GFX_THROW_FAILED(s_pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

        // ----------------------------------------------------------------------------------------------------
        // bind depth state
        s_pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

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
        RAY_GFX_THROW_FAILED(s_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

        // ----------------------------------------------------------------------------------------------------
        // create view of depth stensil texture
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
        descDSV.Format = DXGI_FORMAT_D32_FLOAT;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0u;
        RAY_GFX_THROW_FAILED(s_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_pDSV));

        // ----------------------------------------------------------------------------------------------------
        // bind depth stensil view to OM
        s_pContext->OMSetRenderTargets(1u, m_pTarget.GetAddressOf(), m_pDSV.Get());

        // ----------------------------------------------------------------------------------------------------
        // configure viewport
        D3D11_VIEWPORT vp;
        vp.Width = clientWindowRect.right;
        vp.Height = clientWindowRect.bottom;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        s_pContext->RSSetViewports(1u, &vp);
    }
    
    void Renderer3D::DrawIndexed(uint32_t indicesCount)
    {
        s_pContext->DrawIndexed(indicesCount, 0, 0);
    }

    void Renderer3D::ClearBuffer(float red, float green, float blue)
    {
        const float color[] = { red,green,blue,1.0f };
        s_pContext->ClearRenderTargetView(m_pTarget.Get(), color);
        s_pContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
    }

    void Renderer3D::EndDraw()
    {
        // VSYNC on
        //RAY_GFX_THROW_FAILED(m_pSwapChain->Present(1, 0));

        // VSYNC off
        RAY_GFX_THROW_FAILED(m_pSwapChain->Present(0, 0));
    }

    Renderer3D::~Renderer3D()
    {
        RAY_PROFILE_FUNCTION();
        RAY_LOG_DEBUG("[Renderer3D] Destroyed");
    }
    
    void Renderer3D::OnEvent(Widget* receiver, WindowResizeEvent& e)
    {
        // TEMPORARY! Need customization for near and far
        const Size2& size = e.GetSize();
        SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, size.y / size.x, 0.5f, 500.0f));
    }
}