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

        RL_GFX_THROW_FAILED(s_pIDXGIFactory->CreateSwapChain(s_pDevice.Get(), &sd, &m_pSwapChain));

        WRL::ComPtr<ID3D11Resource> pBackBuffer;
        m_pSwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pBackBuffer);

        RL_GFX_THROW_FAILED(s_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pTarget));
    }
    
    void Renderer3D::DrawIndexed(uint32_t indicesCount)
    {
        // TODO: Move somewhere else (necessary for drawing in multiple windows)
        RECT rc;
        GetClientRect(m_hWnd, &rc);
        D3D11_VIEWPORT vp{};
        vp.Width = rc.right;
        vp.Height = rc.bottom;
        s_pContext->RSSetViewports(1, &vp);
        s_pContext->OMSetRenderTargets(1, m_pTarget.GetAddressOf(), nullptr);

        s_pContext->DrawIndexed(indicesCount, 0, 0);
    }

    void Renderer3D::ClearBuffer(float red, float green, float blue)
    {
        const float color[] = { red,green,blue,1.0f };
        s_pContext->ClearRenderTargetView(m_pTarget.Get(), color);
    }

    void Renderer3D::EndDraw()
    {
        RL_GFX_THROW_FAILED(m_pSwapChain->Present(1, 0));
    }

    Renderer3D::~Renderer3D()
    {
        WRL::ComPtr<ID3D11Debug> pDebug;
        s_pDevice->QueryInterface(__uuidof(ID3D11Debug), &pDebug);
        pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    }
}