#include "RayRender/RRpch.h"

#include "ROpenGLVertexBuffer.h"

#include <RayUtil/RException.h>


namespace At0::Ray
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(std::initializer_list<Vertex> data)
		: m_Strides(sizeof(Vertex))
	{

	}

	void OpenGLVertexBuffer::Bind()
	{

	}
}



