#pragma once

#ifdef _WIN32
#include "../../RIndexBuffer.h"
#include "RDX11GraphicsResources.h"

#include <wrl.h>

struct ID3D11Buffer;


namespace At0::Ray
{
	class RR_API DX11IndexBuffer : public IndexBuffer, DX11GraphicsResources
	{
	public:
		DX11IndexBuffer(std::initializer_list<uint32_t> indices);

		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	};
}

#endif // _WIN32