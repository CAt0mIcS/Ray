#include "rlrpch.h"
#include "PixelShader.h"

#include <RayUtil/TypeConvert.h>
#include <RayUtil/Exception.h>
#include <RayDebug/RInstrumentor.h>

namespace WRL = Microsoft::WRL;

namespace At0::Ray
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