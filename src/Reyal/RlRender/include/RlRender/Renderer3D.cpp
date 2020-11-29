#include "rlrpch.h"
#include "Renderer3D.h"

#include <RlUtil/Exception.h>
#include <RlDebug/RlAssert.h>


namespace WRL = Microsoft::WRL;

namespace At0::Reyal
{
    Renderer3D::Renderer3D()
        : m_pSwapChain(nullptr), m_pTarget(nullptr), m_hWnd(0)
    {

    }

    void Renderer3D::Init(HWND hWnd)
    {
        RL_MEXPECTS(!m_pSwapChain, "D3D11RendererAPI was already initialized.");
        m_hWnd = hWnd;
        
        RECT clientWindowRect;
        GetClientRect(m_hWnd, &clientWindowRect);


        // ----------------------------------------------------------------------------------------------------
        // Swap Chain
        DXGI_SWAP_CHAIN_DESC sd{};

        // Let DXGI figure it out with hWnd
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;

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

        RL_GFX_THROW_FAILED(s_pIDXGIFactory->CreateSwapChain(s_pDevice, &sd, &m_pSwapChain));

        // ----------------------------------------------------------------------------------------------------
        // Back Buffer
        WRL::ComPtr<ID3D11Resource> pBackBuffer;
        m_pSwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pBackBuffer);

        RL_GFX_THROW_FAILED(s_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pTarget));

        // ----------------------------------------------------------------------------------------------------
        // Depth Stencil state
        D3D11_DEPTH_STENCIL_DESC dsDesc{};
        dsDesc.DepthEnable = TRUE;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
        WRL::ComPtr<ID3D11DepthStencilState> pDSState;
        RL_GFX_THROW_FAILED(s_pDevice->CreateDepthStencilState(&dsDesc, &pDSState));
        s_pContext->OMSetDepthStencilState(pDSState.Get(), 1);

        // ----------------------------------------------------------------------------------------------------
        // Create Depth Stencil Texture
        WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
        D3D11_TEXTURE2D_DESC descDepth{};
        descDepth.Width = clientWindowRect.right;
        descDepth.Height = clientWindowRect.bottom;
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        descDepth.Format = DXGI_FORMAT_D32_FLOAT;
        descDepth.SampleDesc.Count = 1;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        RL_GFX_THROW_FAILED(s_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

        // ----------------------------------------------------------------------------------------------------
        // Create View of Depth Stencil Texture
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
        descDSV.Format = DXGI_FORMAT_D32_FLOAT;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        RL_GFX_THROW_FAILED(s_pDevice->CreateDepthStencilView(
            pDepthStencil.Get(), &descDSV, &m_pDSV
        ));
        s_pContext->OMSetRenderTargets(1, m_pTarget.GetAddressOf(), m_pDSV.Get());
    }
    
    void Renderer3D::DrawIndexed(uint32_t indicesCount)
    {
        // TODO: This needs to be done here when drawing in multiple windows (Some of it is in the constructor (duplicates))
        RECT rc;
        GetClientRect(m_hWnd, &rc);
        D3D11_VIEWPORT vp{};
        vp.Width = rc.right;
        vp.Height = rc.bottom;
        s_pContext->RSSetViewports(1, &vp);
        s_pContext->OMSetRenderTargets(1, m_pTarget.GetAddressOf(), m_pDSV.Get());

        s_pContext->DrawIndexed(indicesCount, 0, 0);
    }

    void Renderer3D::ClearBuffer(float red, float green, float blue)
    {
        const float color[] = { red,green,blue,1.0f };
        s_pContext->ClearRenderTargetView(m_pTarget.Get(), color);
        s_pContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    void Renderer3D::EndDraw()
    {
        RL_GFX_THROW_FAILED(m_pSwapChain->Present(1, 0));
    }

    Renderer3D::~Renderer3D()
    {
        
    }
}