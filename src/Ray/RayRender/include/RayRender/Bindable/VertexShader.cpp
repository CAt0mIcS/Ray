#include "rlrpch.h"
#include "VertexShader.h"

#include <RayUtil/TypeConvert.h>
#include <RayUtil/Exception.h>
#include <RayDebug/RInstrumentor.h>


namespace At0::Ray
{
    VertexShader::VertexShader(const std::string_view filepath)
    {
        RAY_PROFILE_FUNCTION();
        // TODO: Abort called here sometimes
        RAY_GFX_THROW_FAILED(D3DReadFileToBlob(Util::MultiByteToWideChar(filepath).c_str(), &m_pBlob));

        RAY_GFX_THROW_FAILED(s_pDevice->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pVertexShader));
    }

    void VertexShader::Bind()
    {
        s_pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    }
    
    ID3DBlob* VertexShader::GetBytecode() const
    {
        RAY_PROFILE_FUNCTION();
        return m_pBlob.Get();
    }
}