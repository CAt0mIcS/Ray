#include "VertexShader.h"

#include "RendererAPI.h"
#include "Platform/D3D11/D3D11VertexShader.h"
#include "Platform/OpenGL/OpenGLVertexShader.h"

#include <RlDebug/RlAssert.h>
#include <RlDebug/ReyalLogger.h>

namespace At0::Reyal
{
    Ref<VertexShader> VertexShader::Create(const std::string_view filepath)
    {
        RL_EXPECTS(RendererAPI::GetAPI() >= RendererAPI::API::FIRST && RendererAPI::GetAPI() <= RendererAPI::API::LAST);

        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:		RL_LOG_WARN("[VertexShader] Returning invalid VertexShader. Calls to the Shader will fail!"); return nullptr;
#ifdef _WIN32
        case RendererAPI::API::D3D11:       return MakeRef<D3D11VertexShader>(filepath);
#endif
        case RendererAPI::API::OpenGL:      return MakeRef<OpenGLVertexShader>(filepath);
        }

        return nullptr;
    }
    
    Ref<VertexShader> VertexShader::Create(const std::string_view name, const std::string_view vertexSrc)
    {
        RL_EXPECTS(RendererAPI::GetAPI() >= RendererAPI::API::FIRST && RendererAPI::GetAPI() <= RendererAPI::API::LAST);
        
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:		RL_LOG_WARN("[VertexShader] Returning invalid VertexShader. Calls to the Shader will fail!"); return nullptr;
#ifdef _WIN32
        case RendererAPI::API::D3D11:       return MakeRef<D3D11VertexShader>(name, vertexSrc);
#endif
        case RendererAPI::API::OpenGL:      return MakeRef<OpenGLVertexShader>(name, vertexSrc);
        }

        return nullptr;
    }
    
    void* VertexShader::GetBufferPointer() const
    {
        RL_LOG_WARN("[VertexShader] GetBufferPointer function was not overwritten.");
        return nullptr;
    }
    
    size_t VertexShader::GetBufferSize() const
    {
        RL_LOG_WARN("[VertexShader] GetBufferSize function was not overwritten.");
        return 0;
    }
}


