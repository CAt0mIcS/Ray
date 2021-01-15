#pragma once

#ifdef _WIN32
#include "../../RVertexBuffer.h"
#include "RDX11GraphicsResources.h"

#include <wrl.h>

struct ID3D11Buffer;


namespace At0::Ray
{
	class RR_API DX11VertexBuffer : public VertexBuffer, DX11GraphicsResources
	{
	public:
		DX11VertexBuffer(std::initializer_list<Vertex> data);

		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
		uint32_t m_Strides;
	};
}

#endif // _WIN32