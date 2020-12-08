#include "rlrpch.h"
#include "PixelShader.h"

#include <RlUtil/TypeConvert.h>
#include <RlUtil/Exception.h>
#include <RlDebug/Instrumentor.h>

namespace WRL = Microsoft::WRL;

namespace At0::Reyal
{
    PixelShader::PixelShader(const std::string_view filepath)
    {
        RL_PROFILE_FUNCTION();
        WRL::ComPtr<ID3DBlob> pBlob;
        RL_GFX_THROW_FAILED(D3DReadFileToBlob(Util::MultiByteToWideChar(filepath).c_str(), &pBlob));

        RL_GFX_THROW_FAILED(s_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader));
    }

    void PixelShader::Bind()
    {
        s_pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
    }
}