#include "../../RRpch.h"
#include "ROpenGLConstantBuffers.h"


namespace At0::Ray
{
	template<typename T>
	OpenGLConstantBuffer<T>::OpenGLConstantBuffer(const T& data)
	{

	}

	template<typename T>
	void OpenGLConstantBuffer<T>::Update(const Matrix& data)
	{

	}

	template<typename T>
	void OpenGLConstantBuffer<T>::Update(const Float3& data)
	{

	}


	template<typename T>
	void OpenGLPixelConstantBuffer<T>::Bind()
	{
	}

	template<typename T>
	void OpenGLVertexConstantBuffer<T>::Bind()
	{
	}


	// explicit instantiation
	template class OpenGLConstantBuffer<Matrix>;
	template class OpenGLConstantBuffer<Float3>;

	template class OpenGLPixelConstantBuffer<Matrix>;
	template class OpenGLPixelConstantBuffer<Float3>;

	template class OpenGLVertexConstantBuffer<Matrix>;
	template class OpenGLVertexConstantBuffer<Float3>;
}