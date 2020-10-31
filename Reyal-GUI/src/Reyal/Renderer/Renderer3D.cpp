#include "rlpch.h"
#include "Renderer3D.h"


namespace At0::Reyal
{
    Renderer3D::Renderer3D()
        : m_API(RendererAPI::Create())
    {

    }

    void Renderer3D::Init(_In_ void* window)
    {
        m_API->Init(window);
    }

    void Renderer3D::RenderTestTriangle()
    {
        m_API->RenderTestTriangle();
    }
    
    void Renderer3D::EndDraw()
    {
        m_API->EndDraw();
    }
}

