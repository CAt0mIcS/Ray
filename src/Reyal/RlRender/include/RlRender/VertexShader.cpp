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
        case RendererAPI::API::D3D11:       return MakeRef<D3D11VertexShader>(filepath);
        case RendererAPI::API::OpenGL:      return MakeRef<OpenGLVertexShader>(filepath);
        }

        return nullptr;
    }
}


