#include "rlrpch.h"
#include "VertexShader.h"

#include <RlUtil/TypeConvert.h>
#include <RlUtil/Exception.h>


namespace At0::Reyal
{
    VertexShader::VertexShader(const std::string_view filepath)
    {
        // TODO: Abort called here sometimes
        RL_GFX_THROW_FAILED(D3DReadFileToBlob(Util::MultiByteToWideChar(filepath).c_str(), &m_pBlob));

        RL_GFX_THROW_FAILED(s_pDevice->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pVertexShader));
    }

    void VertexShader::Bind()
    {
        s_pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    }
    
    ID3DBlob* VertexShader::GetBytecode() const
    {
        return m_pBlob.Get();
    }
}