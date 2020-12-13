#include "RayRender/rlrpch.h"
#include "VertexShader.h"

#include <RayUtil/RTypeConvert.h>
#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>


namespace At0::Ray
{
    VertexShader::VertexShader(std::string_view filepath)
    {
        RAY_PROFILE_FUNCTION();
        // RAY_TODO: Abort called here sometimes
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