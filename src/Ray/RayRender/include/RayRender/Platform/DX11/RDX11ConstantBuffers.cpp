#include "../../RRpch.h"
#include "RDX11ConstantBuffers.h"


namespace At0::Ray
{
	template<typename T>
	DX11ConstantBuffer<T>::DX11ConstantBuffer(const T& data)
	{

	}

	template<typename T>
	void DX11ConstantBuffer<T>::Bind()
	{

	}


	// explicit instantiation
	template class DX11ConstantBuffer<Matrix>;
	template class DX11ConstantBuffer<Float3>;
}