#include "PixelShader.h"

#include "RendererAPI.h"
#include "Platform/D3D11/D3D11PixelShader.h"
#include "Platform/OpenGL/OpenGLPixelShader.h"

#include <RlDebug/RlAssert.h>
#include <RlDebug/ReyalLogger.h>

namespace At0::Reyal
{
    Ref<PixelShader> PixelShader::Create(const std::string_view filepath, FileState state)
    {
        RL_EXPECTS(RendererAPI::GetAPI() >= RendererAPI::API::FIRST && RendererAPI::GetAPI() <= RendererAPI::API::LAST);

        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:		RL_LOG_WARN("[PixelShader] Returning invalid PixelShader. Calls to the Shader will fail!"); return nullptr;
#ifdef _WIN32
        case RendererAPI::API::D3D11:       return MakeRef<D3D11PixelShader>(filepath, state);
#endif
        case RendererAPI::API::OpenGL:      return MakeRef<OpenGLPixelShader>(filepath, state);
        }

        return nullptr;
    }
    
    Ref<PixelShader> PixelShader::Create(const std::string_view name, const std::string_view vertexSrc)
    {
        RL_EXPECTS(RendererAPI::GetAPI() >= RendererAPI::API::FIRST && RendererAPI::GetAPI() <= RendererAPI::API::LAST);
        
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:		RL_LOG_WARN("[PixelShader] Returning invalid PixelShader. Calls to the Shader will fail!"); return nullptr;
#ifdef _WIN32
        case RendererAPI::API::D3D11:       return MakeRef<D3D11PixelShader>(name, vertexSrc);
#endif
        case RendererAPI::API::OpenGL:      return MakeRef<OpenGLPixelShader>(name, vertexSrc);
        }

        return nullptr;
    }
}


