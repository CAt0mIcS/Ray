#include "../../RRpch.h"


#ifdef _WIN32
#include "RDX11ConstantBuffers.h"

#include <RayUtil/RException.h>


namespace At0::Ray
{
	template<typename T>
	DX11ConstantBuffer<T>::DX11ConstantBuffer(const T& data)
	{
		//RAY_ALIGNED(16) struct AlignedData
		//{
		//	T data;
		//};

		D3D11_BUFFER_DESC bd{};
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		//bd.ByteWidth = sizeof(AlignedData);
		bd.ByteWidth = sizeof(T);
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd{};

		//AlignedData alignedData{ data };
		//sd.pSysMem = &alignedData;
		sd.pSysMem = &data;
		RAY_GFX_THROW_FAILED(GetDevice()->CreateBuffer(&bd, &sd, &m_pConstantBuffer));
	}

	template<typename T>
	DX11ConstantBuffer<T>::DX11ConstantBuffer()
	{
		//RAY_ALIGNED(16) struct AlignedData
		//{
		//	T data;
		//};

		D3D11_BUFFER_DESC bd{};
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		//bd.ByteWidth = sizeof(AlignedData);
		bd.ByteWidth = sizeof(T);
		bd.StructureByteStride = 0;
		RAY_GFX_THROW_FAILED(GetDevice()->CreateBuffer(&bd, nullptr, &m_pConstantBuffer));
	}

	template<typename T>
	void DX11ConstantBuffer<T>::Update(const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE msr{};
		RAY_GFX_THROW_FAILED(GetContext()->Map(
			m_pConstantBuffer.Get(), 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr
		));
		memcpy(msr.pData, &data, sizeof(data));
		GetContext()->Unmap(m_pConstantBuffer.Get(), 0);
	}


	template<typename T>
	void DX11PixelConstantBuffer<T>::Bind()
	{
		GetContext()->PSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}

	template<typename T>
	void DX11VertexConstantBuffer<T>::Bind()
	{
		GetContext()->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	}


	// explicit instantiation
	template class DX11ConstantBuffer<Matrix>;
	template class DX11ConstantBuffer<Float3>;

	template class DX11PixelConstantBuffer<Matrix>;
	template class DX11PixelConstantBuffer<Float3>;

	template class DX11VertexConstantBuffer<Matrix>;
	template class DX11VertexConstantBuffer<Float3>;
}

#endif // _WIN32
