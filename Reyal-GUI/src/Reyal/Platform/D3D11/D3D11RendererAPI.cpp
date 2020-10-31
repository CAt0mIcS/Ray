#include "rlpch.h"
#include "D3D11RendererAPI.h"

#include "RlWin.h"

namespace WRL = Microsoft::WRL;


namespace At0::Reyal
{
    void D3D11RendererAPI::Init(_In_ void* window)
    {
        DXGI_SWAP_CHAIN_DESC sd{};

        // Let DXGI figure it out with hWnd
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;

        sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        sd.BufferDesc.RefreshRate = { 0, 0 };
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = static_cast<HWND>(window);
        
        sd.SampleDesc = { 1, 0 };
        sd.BufferCount = 1;
        sd.Windowed = TRUE;

        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;

        //uint32_t creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
        uint32_t creationFlags = 0;
        #ifdef RL_DEBUG
            creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
        #endif

        D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            NULL,
            creationFlags,
            nullptr,
            D3D_FEATURE_LEVEL_11_0,
            D3D11_SDK_VERSION,
            &sd,
            &m_SwapChain,
            &m_Device,
            nullptr,
            &m_Context
        );

        WRL::ComPtr<ID3D11Resource> pBackBuffer;
        m_SwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pBackBuffer);

        m_Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_TargetView);
    }
    
    void D3D11RendererAPI::RenderTestTriangle()
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Vertices ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct Vertex
        {
            struct
            {
                float x, y;
            } pos;
        };

        const Vertex vertices[] =
        {
               {  0.0f,  0.5f },
               {  0.5f, -0.5f },
               { -0.5f, -0.5f }
        };


        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Vertex Buffer ///////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
        D3D11_BUFFER_DESC bd{};
        bd.ByteWidth = sizeof(vertices);
        bd.StructureByteStride = sizeof(Vertex);
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;
        
        D3D11_SUBRESOURCE_DATA sd{};
        sd.pSysMem = vertices;

        m_Device->CreateBuffer(&bd, &sd, &pVertexBuffer);

        uint32_t strides = sizeof(Vertex);
        uint32_t offset = 0;
        m_Context->IASetVertexBuffers(1, 1, pVertexBuffer.GetAddressOf(), &strides, &offset);


        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Pixel Shader ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        WRL::ComPtr<ID3D11PixelShader> pPixelShader;
        WRL::ComPtr<ID3DBlob> pBlob;
        D3DReadFileToBlob(L"", &pBlob);
        m_Device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
        m_Context->PSSetShader(pPixelShader.Get(), nullptr, 0);


        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Vertex Shader ///////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        WRL::ComPtr<ID3D11VertexShader> pVertexShader;
        D3DReadFileToBlob(L"", &pBlob);
        m_Device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
        m_Context->VSSetShader(pVertexShader.Get(), nullptr, 0);


        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Input Layout ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        WRL::ComPtr<ID3D11InputLayout> pLayout;
        D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        m_Device->CreateInputLayout(ied, std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pLayout);


        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Viewport ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        D3D11_VIEWPORT vp;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        vp.MaxDepth = 1;
        vp.MinDepth = 0;
        //vp.Width = 
        //vp.Height = 
        m_Context->RSSetViewports(1, &vp);

        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Primitive Topology //////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Draw Call ///////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        m_Context->Draw(std::size(vertices), 0);
    }
}
