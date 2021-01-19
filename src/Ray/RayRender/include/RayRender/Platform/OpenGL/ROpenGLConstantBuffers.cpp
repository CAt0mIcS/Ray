#include "../../RRpch.h"
#include "ROpenGLConstantBuffers.h"


namespace At0::Ray
{
	template<typename T>
	OpenGLConstantBuffer<T>::OpenGLConstantBuffer(const T& data)
	{

	}

	template<typename T>
	void OpenGLConstantBuffer<T>::Bind()
	{

	}


	// explicit instantiation
	template class OpenGLConstantBuffer<Matrix>;
	template class OpenGLConstantBuffer<Float3>;
}