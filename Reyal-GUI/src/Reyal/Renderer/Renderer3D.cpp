#include "rlpch.h"
#include "Renderer3D.h"

//TODO: Premake doesn't set shader property --> https://stackoverflow.com/questions/55055150/premake5-how-to-build-hlsl-shaders


namespace At0::Reyal
{
    void Renderer3D::Init(HWND hWnd)
    {
        m_hWnd = hWnd;

        DXGI_SWAP_CHAIN_DESC sd{};
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;

        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;

        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;

        sd.OutputWindow = m_hWnd;
        sd.Windowed = TRUE;

        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;

        D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            NULL,
            D3D11_CREATE_DEVICE_DEBUG,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &sd,
            &m_pSwapChain,
            &m_pDevice,
            nullptr,
            &m_pContext
        );

        Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
        m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

        m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pTarget);
    }

    void Renderer3D::RenderTestTriangle()
    {
        ///////////////////////////////////////////////////////////////////////////////////////
        ////////// Vertices ///////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
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


        ///////////////////////////////////////////////////////////////////////////////////////
        ////////// Vertex Buffer //////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

        D3D11_BUFFER_DESC bd{};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;
        bd.StructureByteStride = sizeof(Vertex);
        bd.ByteWidth = sizeof(vertices);

        D3D11_SUBRESOURCE_DATA sd{};
        sd.pSysMem = vertices;

        m_pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
        uint32_t strides = sizeof(Vertex);
        uint32_t offset = 0;
        m_pContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &strides, &offset);


        ///////////////////////////////////////////////////////////////////////////////////////
        ////////// Pixel Shader ///////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
        D3DReadFileToBlob(L"../Reyal-GUI/Shaders/PixelShader-p.cso", &pBlob);
        m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
        m_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);


        ///////////////////////////////////////////////////////////////////////////////////////
        ////////// Vertex Shader //////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
        D3DReadFileToBlob(L"../Reyal-GUI/Shaders/VertexShader-v.cso", &pBlob);
        m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
        m_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);


        ///////////////////////////////////////////////////////////////////////////////////////
        ////////// Input Layout ///////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        Microsoft::WRL::ComPtr<ID3D11InputLayout> pLayout;
        D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        m_pDevice->CreateInputLayout(ied, std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pLayout);
        m_pContext->IASetInputLayout(pLayout.Get());

        ///////////////////////////////////////////////////////////////////////////////////////
        ////////// Primitive Topology /////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        
        ///////////////////////////////////////////////////////////////////////////////////////
        ////////// Viewport ///////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        RECT rc;
        GetClientRect(m_hWnd, &rc);

        D3D11_VIEWPORT vp{};
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        vp.Width = (float)rc.right;
        vp.Height = (float)rc.bottom;
        vp.MinDepth = 0;
        vp.MaxDepth = 1;
        m_pContext->RSSetViewports(1, &vp);


        ///////////////////////////////////////////////////////////////////////////////////////
        ////////// Render Target //////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        m_pContext->OMSetRenderTargets(1, m_pTarget.GetAddressOf(), nullptr);


        m_pContext->Draw(std::size(vertices), 0);
    }
    
    void Renderer3D::EndDraw()
    {
        m_pSwapChain->Present(1, 0);
    }
}
