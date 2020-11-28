#pragma once

#include "RlRBase.h"

#include "GraphicsResource.h"

#include <wrl.h>
#include <Windows.h>

#include <d3d11.h>


namespace At0::Reyal
{
    class RLR_API Renderer3D : private GraphicsResource
    {
    public:
        Renderer3D();
        
        void Init(HWND hWnd);
        void DrawIndexed(uint32_t indicesCount);
        void ClearBuffer(float red, float green, float blue);
        void EndDraw();

    private:
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTarget;

        HWND m_hWnd;
    };
}
