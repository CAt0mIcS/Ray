#include "rlrpch.h"

#ifdef _WIN32

#include "D3D11RendererAPI.h"
#include "D3D11RenderBase.h"

#include <RlUtil/Exception.h>

#include <RlDebug/ReyalLogger.h>
#include <RlDebug/RlAssert.h>
#include <RlDebug/Instrumentor.h>

#include "VertexShader.h"
#include "PixelShader.h"

namespace WRL = Microsoft::WRL;


namespace At0::Reyal
{
    D3D11RendererAPI::D3D11RendererAPI()
        : m_hWnd(NULL), m_SwapChain(nullptr), m_TargetView(nullptr)
    {
        RL_PROFILE_FUNCTION();

        ///////////////////////////////////////////////////////////////////////////////////
        // Create Device and Context
        if (!s_Device || !s_Context)
        {
#ifdef NDEBUG
            uint32_t creationFlags = 0;
#else
            uint32_t creationFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

            RL_GFX_THROW_FAILED(D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                NULL,
                creationFlags,
                nullptr,
                0,
                D3D11_SDK_VERSION,
                &s_Device,
                nullptr,
                &s_Context
            ));
        }

        ///////////////////////////////////////////////////////////////////////////////////
        // Create IDXGIFactory to be able to create SwapChains in D3D11RendererAPI::Init
        if (!s_IDXGIFactory)
        {
            IDXGIDevice* pDevice;
            RL_GFX_THROW_FAILED(s_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice));

            WRL::ComPtr<IDXGIAdapter> pAdapter;
            RL_GFX_THROW_FAILED(pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter));
            pDevice->Release();

            RL_GFX_THROW_FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), &s_IDXGIFactory));

            ///////////////////////////////////////////////////////////////////////////////////
            // Loop over all available IDXGIAdapters to get information about them
            for (uint32_t i = 0; s_IDXGIFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
            {
                DXGI_ADAPTER_DESC adapterDesc;
                pAdapter->GetDesc(&adapterDesc);
                RL_LOG_INFO("[Renderer3D] Found DXGIAdapter: "
                    "\n\tDescription: {0}\n\tVendorID: {1}\n\tDeviceID: {2}\n\tSubSysID: {3}"
                    "\n\tRevision: {4}\n\tDedicated Video Memory: {5}\n\tDedicated System Memory: {6}"
                    "\n\tShared System Memory: {7}\n\tAdapterLuid::LowPart: {8}\n\tAdapterLuid::HighPart: {9}\n",
                    adapterDesc.Description,
                    adapterDesc.VendorId,
                    adapterDesc.DeviceId,
                    adapterDesc.SubSysId,
                    adapterDesc.Revision,
                    adapterDesc.DedicatedVideoMemory,
                    adapterDesc.DedicatedSystemMemory,
                    adapterDesc.SharedSystemMemory,
                    adapterDesc.AdapterLuid.LowPart,
                    adapterDesc.AdapterLuid.HighPart
                );
            }
        }

    }

    void D3D11RendererAPI::Init(void* window)
    {
        RL_PROFILE_FUNCTION();

        RL_MEXPECTS(!this->IsInitialized(), "D3D11RendererAPI was already initialized.");

        m_hWnd = static_cast<HWND>(window);

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

        RL_GFX_THROW_FAILED(s_IDXGIFactory->CreateSwapChain(s_Device.Get(), &sd, &m_SwapChain));

        WRL::ComPtr<ID3D11Resource> pBackBuffer;
        m_SwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pBackBuffer);

        s_Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_TargetView);
    }
    
    bool D3D11RendererAPI::IsInitialized() const
    {
        RL_PROFILE_FUNCTION();

        return m_SwapChain != nullptr && m_TargetView != nullptr;
    }

    void D3D11RendererAPI::RenderTestTriangle()
    {
        RL_PROFILE_FUNCTION();

        RL_MEXPECTS(this->IsInitialized(), "D3D11RendererAPI was not initialized.");
        
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

        s_Device->CreateBuffer(&bd, &sd, &pVertexBuffer);

        uint32_t strides = sizeof(Vertex);
        uint32_t offset = 0;
        s_Context->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &strides, &offset);
        RL_LOG_DEBUG("Bound Vertex Buffer {0}", pVertexBuffer.Get());


        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Pixel Shader ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
#ifdef NDEBUG
        Ref<PixelShader> pixelShader = VertexShader::Create("../../bin/Release/PixelShader-p.cso");
#else
        Ref<PixelShader> pixelShader = VertexShader::Create("../../bin/Debug/PixelShader-p.cso");
#endif
#elif 1
        Ref<PixelShader> pixelShader = PixelShader::Create("../../../src/Reyal/RlRender/Shaders/PixelShader-p.hlsl", FileState::Source);
#else
        std::string vertexSrc = R"(
float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
        )";
        Ref<VertexShader> vertexShader = VertexShader::Create("VertexShader-v", vertexSrc);
#endif
        pixelShader->Bind();
        RL_LOG_DEBUG("Bound Pixel Shader with name \"{0}\"", pixelShader->GetName());


        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Vertex Shader ///////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
#ifdef NDEBUG
        Ref<VertexShader> vertexShader = VertexShader::Create("../../bin/Release/VertexShader-v.cso");
#else
        Ref<VertexShader> vertexShader = VertexShader::Create("../../bin/Debug/VertexShader-v.cso");
#endif
#elif 1
        Ref<VertexShader> vertexShader = VertexShader::Create("../../../src/Reyal/RlRender/Shaders/VertexShader-v.hlsl", FileState::Source);
#else
        std::string vertexSrc = R"(
float4 main( float2 pos : POSITION ) : SV_POSITION
{
    return float4(pos, 0.0, 1.0);
}
        )";
        Ref<VertexShader> vertexShader = VertexShader::Create("VertexShader-v", vertexSrc);
#endif
        vertexShader->Bind();
        RL_LOG_DEBUG("Bound Vertex Shader with name \"{0}\"", vertexShader->GetName());

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Input Layout ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        WRL::ComPtr<ID3D11InputLayout> pLayout;
        D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        s_Device->CreateInputLayout(ied, std::size(ied), vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), &pLayout);
        s_Context->IASetInputLayout(pLayout.Get());
        RL_LOG_DEBUG("Bound Input Layout {0}", pLayout.Get());


        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Viewport ////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        RECT rc;
        GetClientRect(m_hWnd, &rc);

        D3D11_VIEWPORT vp;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        vp.MaxDepth = 1;
        vp.MinDepth = 0;
        vp.Width = (float)rc.right;
        vp.Height = (float)rc.bottom;
        s_Context->RSSetViewports(1, &vp);
        RL_LOG_DEBUG("Bound Viewport {0}", &vp);

        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Primitive Topology //////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        s_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        RL_LOG_DEBUG("Set Primitive Topology to {0}", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Set Render Target ///////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        s_Context->OMSetRenderTargets(1, m_TargetView.GetAddressOf(), nullptr);
        RL_LOG_DEBUG("Set Render Target {0}", m_TargetView.Get());


        /*
        * QUESTION:
        *   Large separators? Improves readability but vertical screen space is limited
        */

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////// Draw Call ///////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        s_Context->Draw(std::size(vertices), 0);
        RL_LOG_DEBUG("Drew Triangle");
    }
    
    void D3D11RendererAPI::EndDraw()
    {
        RL_PROFILE_FUNCTION();

        RL_MEXPECTS(this->IsInitialized(), "D3D11RendererAPI was not initialized.");
        m_SwapChain->Present(1, 0);
    }
}

#endif // _WIN32
