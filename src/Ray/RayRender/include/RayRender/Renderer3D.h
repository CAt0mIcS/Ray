#pragma once

#include "RlRBase.h"

#include "GraphicsResource.h"

#include <wrl.h>
#include <Windows.h>
#include <DirectXMath.h>

#include <../../RayEvent/include/RayEvent/REventListener.h>
#include <../../RayEvent/include/RayEvent/Events/RApplicationEvent.h>


struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace At0::Ray
{
    class RR_API Renderer3D : private GraphicsResource,
        EventListener<WindowResizeEvent>
    {
    public:
        Renderer3D(EventDispatcher<WindowResizeEvent>& dispatcher);
        Renderer3D& operator=(const Renderer3D& other) = delete;
        Renderer3D(const Renderer3D& other) = delete;

        void Init(HWND hWnd);
        void DrawIndexed(uint32_t indicesCount);
        void ClearBuffer(float red, float green, float blue);
        void EndDraw();
        void SetProjection(const DirectX::XMMATRIX& mat) { m_Projection = mat; }
        const DirectX::XMMATRIX& GetProjection() const { return m_Projection; }
        ~Renderer3D();

    private:
        virtual void OnEvent(Widget* receiver, WindowResizeEvent& e) override;

    private:
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTarget;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSV;

        DirectX::XMMATRIX m_Projection;
        HWND m_hWnd;
    };
}
