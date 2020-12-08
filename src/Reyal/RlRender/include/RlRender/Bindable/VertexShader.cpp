#include "rlrpch.h"
#include "VertexShader.h"

#include <RlUtil/TypeConvert.h>
#include <RlUtil/Exception.h>
#include <RlDebug/Instrumentor.h>


namespace At0::Reyal
{
    VertexShader::VertexShader(const std::string_view filepath)
    {
        RL_PROFILE_FUNCTION();
        // TODO: Abort called here sometimes
        RL_GFX_THROW_FAILED(D3DReadFileToBlob(Util::MultiByteToWideChar(filepath).c_str(), &m_pBlob));

        RL_GFX_THROW_FAILED(s_pDevice->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pVertexShader));
    }

    void VertexShader::Bind()
    {
        RL_PROFILE_FUNCTION();
        s_pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    }
    
    ID3DBlob* VertexShader::GetBytecode() const
    {
        RL_PROFILE_FUNCTION();
        return m_pBlob.Get();
    }
}