#pragma once

#ifdef _WIN32

#include "../../RConstantBuffers.h"
#include "Internal/RDX11GraphicsResources.h"

#include <wrl.h>

struct ID3D11Buffer;

namespace At0::Ray
{
	template<typename T>
	class RR_API DX11ConstantBuffer : public ConstantBuffer, protected DX11GraphicsResources
	{
	public:
		DX11ConstantBuffer(const T& data);
		DX11ConstantBuffer();

		virtual void Update(const T& data) override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
	};


	template<typename T>
	class RR_API DX11PixelConstantBuffer : public DX11ConstantBuffer<T>
	{
	public:
		// "Import" constructor from base class
		using DX11ConstantBuffer<T>::DX11ConstantBuffer;

		virtual void Bind() override;
	};


	template<typename T>
	class RR_API DX11VertexConstantBuffer : public DX11ConstantBuffer<T>
	{
	public:
		// "Import" constructor from base class
		using DX11ConstantBuffer<T>::DX11ConstantBuffer;

		virtual void Bind() override;
	};
}

#endif // _WIN32
